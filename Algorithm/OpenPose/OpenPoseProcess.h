#ifndef OPEN_POSE_PROCESS_H_
#define OPEN_POSE_PROCESS_H_

#include "algorithmbase.h"
#include "inifile.h"
#include <vector>


class COpenPoseProcess:public CAlgorithmBase
{
public:
	COpenPoseProcess();

	~COpenPoseProcess();

	int PreProcess();

    cv::Mat Process(cv::Mat& frame);
	
	int PostProcess();

private:

	
private:
    int m_nInputWidth;
    int m_nInputHeight;
    float m_fThreshold;
    string m_strProtoPath;
    string m_strWeightsPath;
    int m_nPoints;
	cv::dnn::Net m_Net;
    std::vector<std::vector<int>> m_PosePairsVec;
};

#endif // OPEN_POSE_PROCESS_H_
