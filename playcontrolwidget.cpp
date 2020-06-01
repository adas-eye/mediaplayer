#include "playcontrolwidget.h"
#include "ui_playcontrolwidget.h"
#include "Singleton.h"
#include "mediaplay.h"
#include <QDebug>
#include <QFileDialog>
#include <QMouseEvent>


PlayControlWidget::PlayControlWidget(QWidget *parent) :
    QWidget(parent),
    m_isNeedHide(true),
    ui(new Ui::PlayControlWidget)

{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setStyleSheet("background-color: rgb(255,255, 255)");
    this->show();

    ui->btnPlayPause->setStyleSheet("background-color:transparent");
    ui->btnPlayPause->setFocusPolicy(Qt::NoFocus);

    ui->btnPrevious->setStyleSheet("background-color:transparent");
    ui->btnPrevious->setFocusPolicy(Qt::NoFocus);

    ui->btnNext->setStyleSheet("background-color:transparent");
    ui->btnNext->setFocusPolicy(Qt::NoFocus);

    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();

    //打开文件
    connect(ui->btnOpenFile, &QToolButton::clicked, [=](){
        if (media->GetCurrentState() == PLAYING)
        {
            media->Pause();
        }
        QStringList fileNames= QFileDialog::getOpenFileNames(this, tr("打开文件"), "D:/", tr("视频文件(*.avi *.mp4)"));
        qDebug() << fileNames;
        media->Add(fileNames);

        if (fileNames.size() == 1)
        {
            media->SetPlayIndex(1);
            if(media->Play() == 0)
            {
                ui->btnPlayPause->setIcon(QIcon(QString(":/res/Pause.png")));
                ui->horizontalSlider->setRange(0, media->GetCurrentTotalFrames());
            }

        }
        show();
    });

    //播放 暂停处理
    connect(ui->btnPlayPause, &QToolButton::clicked, [=](){

        if(media->GetCurrentState() == PLAYING)
        {
            media->Pause();
            ui->btnPlayPause->setIcon(QIcon(QString(":/res/Play.png")));
        }
        else /*if (media->GetCurretnState() == PAUSED)*/
        {
            if( 0 == media->Play())
            {
                ui->btnPlayPause->setIcon(QIcon(QString(":/res/Pause.png")));
            }
        }
        ui->horizontalSlider->setRange(0, media->GetCurrentTotalFrames());
        ui->btnPrevious->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    });

    //播放前一个文件
    connect(ui->btnPrevious, &QToolButton::clicked, [=](){
        media->PlayPrevious();
        ui->horizontalSlider->setRange(0, media->GetCurrentTotalFrames());
    });

    //播放后一个文件
    connect(ui->btnNext, &QToolButton::clicked, [=](){
        media->PlayNext();
        ui->horizontalSlider->setRange(0, media->GetCurrentTotalFrames());
    });

    //停止播放
    connect(ui->btnStop, &QToolButton::clicked, [=](){
        media->Stop();
        ui->btnPrevious->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
        ui->btnPlayPause->setIcon(QIcon(QString(":/res/Play.png")));
    });
}

PlayControlWidget::~PlayControlWidget()
{
    delete ui;
}

void PlayControlWidget::enterEvent(QEvent *event)
{
    this->m_isNeedHide = false;
    QWidget::enterEvent(event);
}

void PlayControlWidget::leaveEvent(QEvent *event)
{
    m_isNeedHide = true;
    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();
    if (media->GetCurrentState() == PLAYING)
    {
        QTimer::singleShot(2500, this, [=](){
            if (!this->isHidden() && m_isNeedHide)
            {
                this->hide();
            }
        });
    }

    QWidget::leaveEvent(event);
}


void PlayControlWidget::show()
{
    QWidget::show();
    std::shared_ptr<MediaPlay> media = Singleton<MediaPlay>::GetInstance();
    if (media->GetPlayList().empty())
    {
        ui->btnPrevious->setEnabled(false);
        ui->btnPlayPause->setEnabled(false);
        ui->btnNext->setEnabled(false);
        ui->btnStop->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }
    else if (media->GetCurrentState() == STOPPED)
    {
        ui->btnPlayPause->setEnabled(true);
        ui->btnStop->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }
    else
    {
        ui->btnPrevious->setEnabled(true);
        ui->btnPlayPause->setEnabled(true);
        ui->btnNext->setEnabled(true);
        ui->btnStop->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
    }

    QTimer::singleShot(2500, this, [=](){
        if (!this->isHidden() && m_isNeedHide)
        {
            this->hide();
        }
    });
}
