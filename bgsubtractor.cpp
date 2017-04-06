#include "bgsubtractor.h"

using namespace cv;

BGSubtractor::BGSubtractor(QObject *parent) :
    QObject(parent),
    m_bgs(createBackgroundSubtractorMOG2(300, 32, true))
{
    // TEST
    //namedWindow("frame");
    //namedWindow("fgMask");
    // TEST
}

BGSubtractor::~BGSubtractor()
{
    // TEST
    //destroyAllWindows();
    // TEST
}

void BGSubtractor::processImage(const Frame& frame)
{
    Frame foregroundMask;
    m_bgs->apply(frame, foregroundMask);

    // Morph: opening = dilate(erode(input))
    erode(foregroundMask, foregroundMask, Frame());
    dilate(foregroundMask, foregroundMask, Frame());

    emit sendForeground(foregroundMask);
}
