
#include <fstream>
#include <sstream>
#include <QDebug>

#include "MaskRcnn/MaskRcnnProcess.h"
#include "Singleton.h"
#include "inifile.h"
#include <opencv2/core/cuda.hpp>

using namespace cv;
using namespace dnn;
using namespace std;

CMaskRcnnProcess::CMaskRcnnProcess(): m_fScoreThreshold(0.0f),m_fMaskThreshold(0.0f), m_usRunType(0),m_strClassesFile(""), m_strColorsFile(""),
m_strModelWeights(""), m_strTextGraph(""), m_strOutputDir("")
{}

CMaskRcnnProcess::~CMaskRcnnProcess()
{
	if (!m_ClassesVec.empty())
	{
		std::vector<std::string>().swap(m_ClassesVec);
	}
	if (!m_ClassesVec.empty())
	{
		std::vector<cv::Scalar>().swap(m_ColorsVec);
	}
}

int CMaskRcnnProcess::PreProcess()
{
    if (RET_OK != Singleton<inifile::IniFile>::GetInstance()->Load("E:/Github/mediaplayer/Algorithm/MaskRcnn/config/config.ini"))
	{
		return -1;
	}
	// Load names of classes
    Singleton<inifile::IniFile>::GetInstance()->GetStringValue("File Config","ClassesFile", &m_strClassesFile);
	ifstream ifs(m_strClassesFile.c_str());
	string line;
	while (getline(ifs, line)) { 
		m_ClassesVec.push_back(line);
	}

	// Load the colors
    Singleton<inifile::IniFile>::GetInstance()->GetStringValue("File Config", "ColorsFile", &m_strColorsFile);
	ifstream colorFptr(m_strColorsFile.c_str());
	while (getline(colorFptr, line)) {
		char* pEnd;
		double r, g, b;
		r = strtod(line.c_str(), &pEnd);
        g = strtod(pEnd, NULL);
		b = strtod(pEnd, NULL);
        //Scalar color = Scalar(r, g, b, 255.0);
		m_ColorsVec.push_back(Scalar(r, g, b, 255.0));
	}

	// Give the configuration and weight files for the model
    Singleton<inifile::IniFile>::GetInstance()->GetStringValue("File Config", "ModelWeights", &m_strModelWeights);
    Singleton<inifile::IniFile>::GetInstance()->GetStringValue("File Config", "TextGraph", &m_strTextGraph);

    qDebug() << "Model Path: " << QString(m_strModelWeights.data()) ;
    qDebug() << "Test Graph:"<< QString(m_strTextGraph.data());

	m_Net = readNetFromTensorflow(m_strModelWeights, m_strTextGraph);
	m_Net.setPreferableBackend(DNN_BACKEND_CUDA);
	m_Net.setPreferableTarget(DNN_TARGET_CUDA);

	return 0;
}


int CMaskRcnnProcess::PostProcess()
{
	return 0;
}

cv::Mat CMaskRcnnProcess::Process(cv::Mat& frame)
{

	Mat blob;

    //判断输入帧是否为空
	if (frame.empty()) {
        return frame;
	}

    //根据输入数据创建blob
	blobFromImage(frame, blob, 1.0, Size(frame.cols, frame.rows), Scalar(), true, false);
	//blobFromImage(frame, blob);

    //设置网络输入
	m_Net.setInput(blob);

    // 运行前向传递以从输出层获取输出
	std::vector<String> outNames(2);
	outNames[0] = "detection_out_final";
	outNames[1] = "detection_masks";
	vector<Mat> outs;
	m_Net.forward(outs, outNames);

    // 为每个检测到的对象提取边界框和掩模（mask）
	DrawProcess(frame, outs);

    // 计算帧率信息并写入到frame中
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = m_Net.getPerfProfile(layersTimes) / freq;
    string label = format("Mask-RCNN inference time for a frame : %0.0f ms", t);
	putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));

    return frame;
}


// Draw the predicted bounding box, colorize and show the mask on the image
void CMaskRcnnProcess::DrawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask)
{
	//Draw a rectangle displaying the bounding box
	rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 3);

	//Get the label for the class name and its confidence
	string label = format("%.2f", conf);
	if (!m_ClassesVec.empty())
	{
        CV_Assert(classId < m_ClassesVec.size());
		label = m_ClassesVec[classId] + ":" + label;
	}

	//Display the label at the top of the bounding box
	int baseLine;
	Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	box.y = max(box.y, labelSize.height);
	rectangle(frame, Point(box.x, box.y - round(1.5*labelSize.height)), Point(box.x + round(1.5*labelSize.width), box.y + baseLine), Scalar(255, 255, 255), FILLED);
	putText(frame, label, Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);

	Scalar color = m_ColorsVec[classId % m_ColorsVec.size()];

	// Resize the mask, threshold, color and apply it on the image
	resize(objectMask, objectMask, Size(box.width, box.height));
	Mat mask = (objectMask > m_fMaskThreshold);
	Mat coloredRoi = (0.3 * color + 0.7 * frame(box));
	coloredRoi.convertTo(coloredRoi, CV_8UC3);

	// Draw the contours on the image
	vector<Mat> contours;
	Mat hierarchy;
	mask.convertTo(mask, CV_8U);
	findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
	coloredRoi.copyTo(frame(box), mask);

}

// For each frame, extract the bounding box and mask for each detected object
void CMaskRcnnProcess::DrawProcess(Mat& frame, const vector<Mat>& outs)
{
	Mat outDetections = outs[0];
	Mat outMasks = outs[1];

	// Output size of masks is NxCxHxW where
	// N - number of detected boxes
	// C - number of classes (excluding background)
	// HxW - segmentation shape
	const int numDetections = outDetections.size[2];
    //const int numClasses = outMasks.size[1];

    outDetections = outDetections.reshape(1, static_cast<int>(outDetections.total()) / 7);
	for (int i = 0; i < numDetections; ++i)
	{
		float score = outDetections.at<float>(i, 2);
		if (score > m_fScoreThreshold)
		{
			// Extract the bounding box
			int classId = static_cast<int>(outDetections.at<float>(i, 1));
			int left = static_cast<int>(frame.cols * outDetections.at<float>(i, 3));
			int top = static_cast<int>(frame.rows * outDetections.at<float>(i, 4));
			int right = static_cast<int>(frame.cols * outDetections.at<float>(i, 5));
			int bottom = static_cast<int>(frame.rows * outDetections.at<float>(i, 6));

			left = max(0, min(left, frame.cols - 1));
			top = max(0, min(top, frame.rows - 1));
			right = max(0, min(right, frame.cols - 1));
			bottom = max(0, min(bottom, frame.rows - 1));
			Rect box = Rect(left, top, right - left + 1, bottom - top + 1);

			// Extract the mask for the object
			Mat objectMask(outMasks.size[2], outMasks.size[3], CV_32F, outMasks.ptr<float>(i, classId));

			// Draw bounding box, colorize and show the mask on the image
			DrawBox(frame, classId, score, box, objectMask);

		}
	}
}

/*int CMaskRcnnProcess::LoadRunType()
{
	// Open a video file or an image file or a camera stream.
	std::string strFilePath("");

	try {

		if (1 == m_usRunType)
		{
			// Open the image file
			m_Param.GetStringValue("Image Config", "ImageFile", &strFilePath);
			//cout << "Image file input : " << str << endl;
			ifstream ifile(strFilePath);
			if (!ifile) throw("error");
			m_VideoCapture.open(strFilePath);
			strFilePath.replace(strFilePath.end() - 4, strFilePath.end(), "_mask_rcnn_out.jpg");
			m_strOutputDir = strFilePath;
			return 0;
		}
		else if (2 == m_usRunType)
		{
			// Open the video file

			m_Param.GetStringValue("Video Config", "VideoFile", &strFilePath);
			ifstream ifile(strFilePath);
			if (!ifile) {
				throw("error");
			}
			m_VideoCapture.open(strFilePath);
			strFilePath.replace(strFilePath.end() - 4, strFilePath.end(), "_mask_rcnn_out.avi");
			m_strOutputDir = strFilePath;
			return 0;
		}
		// Open the webcam
		else if (0 == m_usRunType)
		{
			int device(-1);
			m_Param.GetIntValue("Device Config", "Device", &device);

			m_VideoCapture.open(device);
			m_Param.GetStringValue("Device Config", "OutputDir", &m_strOutputDir);
			return 0;
		}

	}
	catch (...) {
		cout << "Could not open the input image/video stream" << endl;
		return 0;
	}
	return -1;
}*/
