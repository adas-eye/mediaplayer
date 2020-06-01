#include "OpenPose/OpenPoseProcess.h"
#include "Singleton.h"
#include <QDebug>

using namespace cv;
using namespace cv::dnn;

COpenPoseProcess::COpenPoseProcess()
{}

COpenPoseProcess::~COpenPoseProcess()
{
    std::vector<std::vector<int>>().swap(m_PosePairsVec);
}

int COpenPoseProcess::PreProcess()
{
    if (RET_OK != Singleton<inifile::IniFile>::GetInstance()->Load("E:/Github/mediaplayer/Algorithm/OpenPose/config/config.ini"))
    {
        return -1;
    }

    Singleton<inifile::IniFile>::GetInstance()->GetIntValue("Input","InputWith", &m_nInputWidth);
    Singleton<inifile::IniFile>::GetInstance()->GetIntValue("Input","InputHeight", &m_nInputHeight);
    Singleton<inifile::IniFile>::GetInstance()->GetFloatValue("Input","Threshold", &m_fThreshold);

    std::string strModelType;
    Singleton<inifile::IniFile>::GetInstance()->GetStringValue("Model Type","Type", &strModelType);
    if (strModelType == "MPI")
    {
        Singleton<inifile::IniFile>::GetInstance()->GetStringValue("MPI","ProtoFile", &m_strProtoPath);
        Singleton<inifile::IniFile>::GetInstance()->GetStringValue("MPI","WeightsFile", &m_strWeightsPath);
        Singleton<inifile::IniFile>::GetInstance()->GetIntValue("MPI","TotalPoint", &m_nPoints);

        m_PosePairsVec.resize(m_nPoints - 1);
        if (m_nPoints == 15){
            std::vector<int> poseVec(2);
            poseVec[0] = 0;
            poseVec[1] = 1;
            m_PosePairsVec[0] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 2;
            m_PosePairsVec[1] = poseVec;
            poseVec[0] = 2;
            poseVec[1] = 3;
            m_PosePairsVec[2] = poseVec;
            poseVec[0] = 3;
            poseVec[1] = 4;
            m_PosePairsVec[3] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 5;
            m_PosePairsVec[4] = poseVec;
            poseVec[0] = 5;
            poseVec[1] = 6;
            m_PosePairsVec[5] = poseVec;
            poseVec[0] = 6;
            poseVec[1] = 7;
            m_PosePairsVec[6] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 14;
            m_PosePairsVec[7] = poseVec;
            poseVec[0] = 14;
            poseVec[1] = 8;
            m_PosePairsVec[8] = poseVec;
            poseVec[0] = 8;
            poseVec[1] = 9;
            m_PosePairsVec[9] = poseVec;
            poseVec[0] = 9;
            poseVec[1] = 10;
            m_PosePairsVec[10] = poseVec;
            poseVec[0] = 14;
            poseVec[1] = 11;
            m_PosePairsVec[11] = poseVec;
            poseVec[0] = 11;
            poseVec[1] = 12;
            m_PosePairsVec[12] = poseVec;
            poseVec[0] = 12;
            poseVec[1] = 13;
            m_PosePairsVec[13] = poseVec;
        }
    }
    else if (strModelType == "COCO")
    {
        Singleton<inifile::IniFile>::GetInstance()->GetStringValue("COCO","ProtoFile", &m_strProtoPath);
        Singleton<inifile::IniFile>::GetInstance()->GetStringValue("COCO","WeightsFile", &m_strWeightsPath);
        Singleton<inifile::IniFile>::GetInstance()->GetIntValue("COCO","TotalPoint", &m_nPoints);

        m_PosePairsVec.resize(m_nPoints - 1);
        if (m_nPoints == 18){
            std::vector<int> poseVec(2);
            poseVec[0] = 0;
            poseVec[1] = 1;
            m_PosePairsVec[0] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 2;
            m_PosePairsVec[1] = poseVec;
            poseVec[0] = 2;
            poseVec[1] = 3;
            m_PosePairsVec[2] = poseVec;
            poseVec[0] = 3;
            poseVec[1] = 4;
            m_PosePairsVec[3] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 5;
            m_PosePairsVec[4] = poseVec;
            poseVec[0] = 5;
            poseVec[1] = 6;
            m_PosePairsVec[5] = poseVec;
            poseVec[0] = 6;
            poseVec[1] = 7;
            m_PosePairsVec[6] = poseVec;
            poseVec[0] = 1;
            poseVec[1] = 14;
            m_PosePairsVec[7] = poseVec;
            poseVec[0] = 14;
            poseVec[1] = 8;
            m_PosePairsVec[8] = poseVec;
            poseVec[0] = 8;
            poseVec[1] = 9;
            m_PosePairsVec[9] = poseVec;
            poseVec[0] = 9;
            poseVec[1] = 10;
            m_PosePairsVec[10] = poseVec;
            poseVec[0] = 14;
            poseVec[1] = 11;
            m_PosePairsVec[11] = poseVec;
            poseVec[0] = 11;
            poseVec[1] = 12;
            m_PosePairsVec[12] = poseVec;
            poseVec[0] = 12;
            poseVec[1] = 13;
            m_PosePairsVec[13] = poseVec;
            poseVec[0] = 14;
            poseVec[1] = 16;
            m_PosePairsVec[14] = poseVec;
            poseVec[0] = 0;
            poseVec[1] = 15;
            m_PosePairsVec[15] = poseVec;
            poseVec[0] = 15;
            poseVec[1] = 17;
            m_PosePairsVec[16] = poseVec;
        }
    }
    
    m_Net = readNetFromCaffe(m_strProtoPath, m_strWeightsPath);
    m_Net.setPreferableBackend(DNN_BACKEND_CUDA);
    m_Net.setPreferableTarget(DNN_TARGET_CUDA);
    return 0;
}

cv::Mat COpenPoseProcess::Process(cv::Mat& frame)
{
	int frameWidth = frame.cols;
    int frameHeight = frame.rows;
	
    double t = static_cast<double>(cv::getTickCount());
	
    Mat inpBlob = blobFromImage(frame, 1.0 / 255, Size(m_nInputWidth, m_nInputHeight), Scalar(0, 0, 0), false, false);

    m_Net.setInput(inpBlob);

    Mat output = m_Net.forward();

    int H = output.size[2];
    int W = output.size[3];

    // find the position of the body parts
    vector<Point> points(m_nPoints);
    for (int n=0; n < m_nPoints; n++)
    {
        // Probability map of corresponding body's part.
        Mat probMap(H, W, CV_32F, output.ptr(0,n));

        Point2f p(-1,-1);
        Point maxLoc;
        double prob;
        minMaxLoc(probMap, 0, &prob, 0, &maxLoc);
        if (prob > m_fThreshold)
        {
            p = maxLoc;
            p.x *= (float)frameWidth / W ;
            p.y *= (float)frameHeight / H ;

            circle(frame, cv::Point((int)p.x, (int)p.y), 8, Scalar(0,255,255), -1);
            cv::putText(frame, cv::format("%d", n), cv::Point((int)p.x, (int)p.y), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 2);

        }
        points[n] = p;
    }

    size_t nPairs = m_PosePairsVec.size();

    for (size_t n = 0; n < nPairs; n++)
    {
        // lookup 2 connected body/hand parts
        Point2f partA = points[m_PosePairsVec[n][0]];
        Point2f partB = points[m_PosePairsVec[n][1]];

        if (partA.x<=0 || partA.y<=0 || partB.x<=0 || partB.y<=0)
            continue;

        line(frame, partA, partB, Scalar(0,255,255), 8);
        circle(frame, partA, 8, Scalar(0,0,255), -1);
        circle(frame, partB, 8, Scalar(0,0,255), -1);
    }
    
    t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
    qDebug() << "Time Taken = " << t ;
	return frame;
}
	
int COpenPoseProcess::PostProcess()
{
    return 0;
}
