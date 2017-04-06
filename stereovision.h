#ifndef STEREOVISION_H
#define STEREOVISION_H

#include <QObject>
#include <QTimer>

#include "stereocamera.h"
#include "helpertypes.h"

class StereoVision : public QObject
{
    Q_OBJECT
public:
    explicit StereoVision(QObject *parent = 0);
    ~StereoVision();

signals:
    void provideImage(const Frame &);

public slots:
    void startRecording();

private slots:
    void captureFrame();

private:
    void processFrames(const FramePair &);
    void calibrateCamera();

    StereoCamera m_camera;

    QTimer* m_captureTimer;

    // Calibrated undistort/rectify maps
    // of cameras (x1/y1, x2/y2)
    QVector<cv::Mat> m_stereoSystem;
    QVector<cv::Mat> m_cameras;
    cv::Mat m_transformationMatrix;
};

#endif // STEREOVISION_H
