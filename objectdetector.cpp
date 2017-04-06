#include "objectdetector.h"

using namespace cv;
using namespace std;

ObjectDetector::ObjectDetector(const std::shared_ptr<Scene> scene, QObject *parent) :
    QObject(parent),
    m_scene(scene)
{
    SimpleBlobDetector::Params params;
    params.minDistBetweenBlobs = 500.0f;
    params.minThreshold = 192.0f;
    params.thresholdStep = 20.0f;
    params.filterByInertia = false;
    params.filterByConvexity = false;
    params.filterByColor = false;
    params.blobColor = 255;
    params.filterByCircularity = false;
    params.filterByArea = true;
    params.minArea = 200.0f;
    params.maxArea = 500.0f;

    m_sbd = SimpleBlobDetector::create(params);

    namedWindow("foreground", WINDOW_AUTOSIZE);
}

ObjectDetector::~ObjectDetector()
{
    destroyAllWindows();
}

void ObjectDetector::processForeground(const Frame& fg)
{
    if (fg.empty())
    {
        qDebug() << "Error: empty input foreground mask!";
        return;
    }

    vector<KeyPoint> objects;
    m_sbd->detect(fg, objects);

    //qDebug() << "detected keypoints:" << objects.size();

    //foreach (const auto& obj, objects)
    //{
    //    qDebug() << "X:" << obj.pt.x;
    //    qDebug() << "Y:" << obj.pt.y;
    //}

    Frame objImg;
    //vector<vector<Point> > contours;
    //findContours(fg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    //drawContours(objImg, contours, -1, Scalar(0,0,255), 2);

    drawKeypoints(fg, objects, objImg);
    if (!objImg.empty())
        imshow("foreground", objImg);
}
