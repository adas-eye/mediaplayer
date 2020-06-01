#ifndef ALGORITHMBASE_H
#define ALGORITHMBASE_H

#include <opencv2/opencv.hpp>

typedef enum {
    ALGORITHM_DEFAULT,
    ALGORITHM_MASK_RCNN,
    ALGORITHM_OPEN_POSE,
    ALGORITHM_BUTT
}AlgorithmEnum;

class CAlgorithmBase
{
public:
    CAlgorithmBase();

    virtual ~CAlgorithmBase() = 0;

    virtual int PreProcess() = 0;

    virtual cv::Mat Process(cv::Mat& frame) = 0;

    virtual int PostProcess() = 0;
};

#endif // ALGORITHMBASE_H
