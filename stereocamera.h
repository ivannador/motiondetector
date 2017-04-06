#ifndef STEREOCAMERA_H
#define STEREOCAMERA_H

#include "helpertypes.h"

class StereoCamera
{
public:
    StereoCamera();
    ~StereoCamera();

    bool startCameras();
    FramePair capture();

private:
    CameraPair m_cams;

};

#endif // STEREOCAMERA_H
