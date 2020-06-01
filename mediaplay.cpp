#include "Common.h"
#include "mediaplay.h"
#include "MaskRcnn/MaskRcnnProcess.h"
#include "OpenPose/OpenPoseProcess.h"
#include <Windows.h>
#include <QDebug>
#include <iostream>


using namespace cv;
using namespace media;

MediaPlay::MediaPlay():
    m_AlgorithmType(ALGORITHM_OPEN_POSE)
  , m_PlayState(STOPPED)
  , m_currentPlayFrame(0)
  , m_total(0)
  , m_currentIndex(0)
  , m_pCapture(nullptr)
  , m_LabelShow(nullptr)
  , m_pAlgorithm(nullptr)

{
    m_pTimer = new QTimer;
    this->m_pTimer->setParent(this);
    LoadAlgorithm(m_AlgorithmType);
}

MediaPlay::~MediaPlay()
{
   if (m_pCapture != nullptr)
   {
       m_pCapture->release();
       SAFE_DELETE_PTR(m_pCapture);
   }
   SAFE_DELETE_PTR(m_LabelShow);
   SAFE_DELETE_PTR(m_pTimer);

   std::map<unsigned, std::string>().swap( m_playMap);

   SAFE_DELETE_PTR(m_pAlgorithm);

//    AlgorithmVec::iterator itBegin =  m_AlgorithmObjVec.begin();
//    for(;itBegin != m_AlgorithmObjVec.end(); ++itBegin)
//    {
//        (*itBegin)->PostProcess();
//        SAFE_DELETE_PTR(*itBegin);
//    }
//    AlgorithmVec().swap(m_AlgorithmObjVec);
}

void MediaPlay::Add(const QStringList& strPathlist)
{
    QList<QString>::const_iterator itBegin = strPathlist.begin();
    for (;itBegin != strPathlist.end(); ++itBegin)
    {
        Add(*itBegin);
    }
}

void MediaPlay::Add(const QString& strPath)
{
    //this->m_playMap.insert(m_total++, strPath);
    m_playMap[m_total++] = strPath.toStdString();
    //std::cout << "++++++++" << m_playMap[m_total++] << "++++++++" << std::endl;
    //std::cout << "--------" << strPath.toStdString() << "--------"<< std::endl;
}

void MediaPlay::Delete(unsigned index)
{
    std::map<unsigned, std::string>::iterator itFind = this->m_playMap.find(index);
    if (itFind == this->m_playMap.end())
    {
        return ;
    }
    if (itFind->first == this->m_currentIndex)
    {
        this->m_currentIndex--;
    }
    this->m_playMap.erase(itFind);
    this->m_total--;
}

void MediaPlay::Delete(const QString& sPath)
{
    std::string strPath = sPath.toStdString();
    std::map<unsigned, std::string>::iterator itBegin = this->m_playMap.begin();
    for(;itBegin != this->m_playMap.end();)
    {
        if (strPath == itBegin->second)
        {
            if (itBegin->first == this->m_currentIndex)
            {
                this->m_currentIndex--;
            }
            this->m_playMap.erase(itBegin);
            this->m_total--;
            break;
        }
    }
}

int MediaPlay::Play()
{
    return Play(this->m_currentIndex);
}

int MediaPlay::Play(unsigned index)
{
    if (m_PlayState == PLAYING)
    {
        return 0;
    }
    else if (m_PlayState == PAUSED)
    {
        this->m_pTimer->start();
    }
    else if(m_PlayState == STOPPED)
    {
        if(0 == InitCapture())
        {
            connect(m_pTimer, &QTimer::timeout, [=](){
                std::lock_guard<std::mutex> gLock(m_mutex);
                RunCapture();
            });
            std::lock_guard<std::mutex> gLock(m_mutex);
            m_pTimer->start(int(1000 / this->m_pCapture->get(CAP_PROP_FPS)));
        }
        else
        {
            return -1;
        }
    }
    m_PlayState = PLAYING;
    this->m_currentIndex = index;

    if (m_pAlgorithm != nullptr)
    {
        return m_pAlgorithm->PreProcess();
    }
    return 0;
}

void MediaPlay::PlayPrevious()
{
    Stop();
    if (this->m_currentIndex > 1)
    {
        this->m_currentIndex--;
    }
    else
    {
        this->m_currentIndex = 1;
    }
    Play(this->m_currentIndex);
}

void MediaPlay::PlayNext()
{
    Stop();
    if (this->m_currentIndex < 1)
    {
        return ;
    }
    else if (this->m_currentIndex < this->m_playMap.size())
    {
        this->m_currentIndex++;
    }
    else
    {
        this->m_currentIndex = 1;
    }
    Play(this->m_currentIndex);
}

void MediaPlay::Pause()
{
    if (m_PlayState == PLAYING)
    {
        this->m_pTimer->stop();
        m_PlayState = PAUSED;
    }
}

void MediaPlay::Stop()
{
    if (m_PlayState == STOPPED)
    {
        return ;
    }
    else
    {
        this->m_pTimer->stop();
        ReleaseCapture();
    }
    this->m_LabelShow->clear();
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0));
    this->m_LabelShow->setAutoFillBackground(true);
    this->m_LabelShow->setPalette(palette);
    m_PlayState = STOPPED;
}

void MediaPlay::VolumeUp()
{}

void MediaPlay::VolumeDown()
{}

unsigned MediaPlay::GetCurrentPlayIndex()
{
    return m_currentIndex;
}

int MediaPlay::GetCurrentState()
{
    return m_PlayState;
}

int MediaPlay::GetCurrentTotalFrames()
{
    std::lock_guard<std::mutex> gLock(m_mutex);
    return int(this->m_pCapture->get(CAP_PROP_FRAME_COUNT));
}

unsigned int MediaPlay::GetCurrentFrame()
{
    std::lock_guard<std::mutex> gLock(m_mutex);
    return this->m_currentPlayFrame;
}

const std::map<unsigned, std::string>& MediaPlay::GetPlayList()
{
    return this->m_playMap;
}

void MediaPlay::SetPlayPosition(int pos)
{
    std::lock_guard<std::mutex> gLock(m_mutex);
    if (!this->m_pCapture->isOpened())
    {
        return ;
    }
    this->m_pCapture->set(CAP_PROP_POS_FRAMES, pos);
    this->m_currentPlayFrame = unsigned (pos);

    RunCapture();
}

bool MediaPlay::IsStoped()
{
    if (m_PlayState == STOPPED)
    {
        return true;
    }
    return false;
}

bool MediaPlay::IsPaused()
{
    if (m_PlayState == PAUSED)
    {
        return true;
    }
    return false;
}

int MediaPlay::InitCapture()
{
    //qDebug()<< "播放map: ["<< this->m_playMap <<"]";
    if (this->m_playMap.empty())
    {
        return -1;
    }
    //自动初始化播放索引为1
    if(0 == this->m_currentIndex)
    {
        this->m_currentIndex = 1;
    }

    std::string strPlayFile= this->m_playMap[this->m_currentIndex-1];

    //std::string strPlayFile = "D:/02_cao-zuo-xi-tong-de-ding-yi-he-zuo-yong.mp4"/*std::string(ba.data())*/;

     //std::cout << "=====["<< this->m_playMap[this->m_currentIndex - 1] <<"]=====";
     std::lock_guard<std::mutex> gLock(m_mutex);
    if (this->m_pCapture == nullptr)
    {
        this->m_pCapture = new cv::VideoCapture(strPlayFile);
    }
    else
    {
        this->m_pCapture->open(strPlayFile);
    }

    if (!this->m_pCapture->isOpened())
    {
        qDebug()<< "打开文件 ["<< QString(this->m_playMap[this->m_currentIndex - 1].data()) <<"] 失败";
        return -1;
    }
    m_currentPlayFrame = 0;
    return 0;
}

void MediaPlay::RunCapture()
{
    if (this->m_LabelShow == nullptr)
    {
        return ;
    }

    Mat frame;
    this->m_pCapture->read(frame);
    if (frame.empty())
    {
        return ;
    }
    m_currentPlayFrame++;

    //TODO:opencv 识别处理
    if (m_pAlgorithm != nullptr)
    {
        m_pAlgorithm->Process(frame);
    }

//    AlgorithmVec::iterator itBegin =  m_AlgorithmObjVec.begin();
//    for(;itBegin != m_AlgorithmObjVec.end(); ++itBegin)
//    {
//        frame = (*itBegin)->Process(frame);
//    }

    //TODO:图片格式对应适配

    QPixmap tmpPix = QPixmap::fromImage(QImage(frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped());
    this->m_LabelShow->setGeometry(0, 0, tmpPix.width(), tmpPix.height());
    this->m_LabelShow->setPixmap(tmpPix);
    this->m_LabelShow->setAlignment(Qt::AlignCenter);
}

void MediaPlay::ReleaseCapture()
{
    std::lock_guard<std::mutex> gLock(m_mutex);
    if (!m_pCapture->isOpened())
    {
        m_pCapture->release();
    }
    this->m_PlayState = STOPPED;
}

void MediaPlay::LoadAlgorithm(AlgorithmEnum type)
{
    try {
        switch(type)
        {
        case ALGORITHM_MASK_RCNN:
            m_pAlgorithm = new CMaskRcnnProcess();
            break;
        case ALGORITHM_OPEN_POSE:
            m_pAlgorithm = new COpenPoseProcess();
            break;
        default:
            break;
        }

    } catch (...) {
        qDebug() << "Error: Load algorithm error!";
        exit(-1);
    }
}

int MediaPlay::SetShowLabel(QLabel* pUi)
{
    if (pUi != nullptr)
    {
        this->m_LabelShow = pUi;
        return 0;
    }
    else
    {
        return -1;
    }
}

void MediaPlay::SetPlayIndex(qint16 index)
{
    this->m_currentIndex = quint32(index);
}


void MediaPlay::SetAlgorithmType(AlgorithmEnum type)
{
    this->m_AlgorithmType = type;
}
