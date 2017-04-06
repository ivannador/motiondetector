#include "stereocamera.h"

using namespace std;

StereoCamera::StereoCamera()
{
}

StereoCamera::~StereoCamera()
{
}

bool StereoCamera::startCameras()
{
    return (m_cams.first.open(1) && m_cams.second.open(2));
}

FramePair StereoCamera::capture()
{
    if (!m_cams.first.isOpened() || !m_cams.second.isOpened())
    {
        qDebug() << "Error: cameras are not opened!";
        return make_pair(Frame(), Frame());
    }

    // In a multiple camera environment
    // Camera::read() should not be called,
    // as it does decoding, which has a time-cost
    // Handle grab() and retrieve() separately, so frames
    // can be as close in time as possible
    //
    // If either of the cameras fail, return empty frames,
    // as frames are evaluable only in pairs
    if (!m_cams.first.grab() || !m_cams.second.grab())
    {
        qDebug() << "Error: could not grab frames from cameras!";
        return make_pair(Frame(), Frame());
    }

    Frame frame1, frame2;
    if (!m_cams.first.retrieve(frame1) || !m_cams.second.retrieve(frame2))
    {
        qDebug() << "Error: could not grab frames from cameras!";
        return make_pair(Frame(), Frame());
    }

    return make_pair(frame1, frame2);
}
