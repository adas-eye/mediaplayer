#ifndef MASK_RCNN_PROCESS_H_
#define MASK_RCNN_PROCESS_H_

#include "algorithmbase.h"
#include "inifile.h"
#include <string>


class CMaskRcnnProcess:public CAlgorithmBase
{
public:
	CMaskRcnnProcess();

	~CMaskRcnnProcess();

	int PreProcess();

    cv::Mat Process(cv::Mat& frame);
	
	int PostProcess();

private:

    //int Processing();

    //int LoadRunType();

	// Draw the predicted bounding box
	void DrawBox(cv::Mat& frame, int classId, float conf, cv::Rect box, cv::Mat& objectMask);

	// Postprocess the neural network's output for each frame
	void DrawProcess(cv::Mat& frame, const std::vector<cv::Mat>& outs);

	
private:
	// Initialize the parameters
	float m_fScoreThreshold; // Confidence threshold

	float m_fMaskThreshold; // Mask threshold

	int m_usRunType;

	std::string m_strClassesFile;
	
	std::string m_strColorsFile;
	
	std::string m_strModelWeights;
	
	std::string m_strTextGraph;
	
	std::string m_strOutputDir;

    //inifile::IniFile m_Param;

    //cv::VideoCapture m_VideoCapture;

    //cv::VideoWriter m_VideoWriter;

	std::vector<std::string> m_ClassesVec;

	std::vector<cv::Scalar> m_ColorsVec;

	cv::dnn::Net m_Net;
};

#endif //MASK_RCNN_PROCESS_H_
