#ifndef HELPERTYPES_H
#define HELPERTYPES_H

#include <memory>

#include <QImage>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

typedef cv::Ptr<cv::StereoBM> SBMPtr;
typedef cv::Ptr<cv::BackgroundSubtractorMOG2> BGSPtr;
typedef cv::Ptr<cv::SimpleBlobDetector> SBDPtr;

typedef cv::Mat Frame;
typedef std::pair<cv::Mat, cv::Mat> FramePair;
Q_DECLARE_METATYPE(Frame)

typedef cv::VideoCapture Camera;
typedef std::pair<Camera, Camera> CameraPair;

inline cv::Mat QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result;
    cvtColor(tmp, result, CV_BGR2RGB);
    return result;
}

#endif // HELPERTYPES_H
