#include "manager.h"

#include "stereovision.h"
#include "bgsubtractor.h"
#include "objectdetector.h"

#include "testimageprovider.h"
#include <QTimer>

Manager::Manager(QObject *parent) :
    QObject(parent),
    m_scene(new Scene())
{
    qDebug() << "Constructing Manager...";

    // Needed to be able to pass Frame in queued connections
    qRegisterMetaType<Frame>();

    auto imageProvider = new TestImageProvider();
    auto stereoVision = new StereoVision();
    auto bgSubtractor = new BGSubtractor();
    auto objectDetector = new ObjectDetector(m_scene);

    //connect(imageProvider, &TestImageProvider::provideImage,
    //        bgSubtractor, &BGSubtractor::processImage,
    //        Qt::QueuedConnection);
    connect(stereoVision, &StereoVision::provideImage,
            bgSubtractor, &BGSubtractor::processImage,
            Qt::QueuedConnection);

    // Start parsing images after event loop is started (1 sec is enough)
    QTimer::singleShot(1000, stereoVision, SLOT(startRecording()));
    //QTimer::singleShot(1000, imageProvider, SLOT(parseImages()));


    imageProvider->moveToThread(&m_captureThread);
    stereoVision->moveToThread(&m_captureThread);
    bgSubtractor->moveToThread(&m_detectorThread);
    objectDetector->moveToThread(&m_detectorThread);

    connect(&m_captureThread, &QThread::finished,
            imageProvider, &QObject::deleteLater);
    connect(&m_captureThread, &QThread::finished,
            stereoVision, &QObject::deleteLater);
    connect(&m_detectorThread, &QThread::finished,
            bgSubtractor, &QObject::deleteLater);
    connect(&m_detectorThread, &QThread::finished,
            objectDetector, &QObject::deleteLater);

    connect(bgSubtractor, &BGSubtractor::sendForeground,
            objectDetector, &ObjectDetector::processForeground);

    qDebug() << "Starting threads...";

    m_captureThread.start();
    m_detectorThread.start();
}

Manager::~Manager()
{
    m_captureThread.quit();
    m_detectorThread.quit();
    m_captureThread.wait();
    m_detectorThread.wait();
}
