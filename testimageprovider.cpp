#include "testimageprovider.h"

#include <QDir>
#include <QDebug>

void TestImageProvider::parseImages()
{
    QDir imageDir("D:\\Devel\\qt_projects\\MotionDetector\\Demo_001");

    auto imageList = imageDir.entryInfoList(QDir::Files, QDir::Name);
    if (imageList.empty())
        return;

    foreach (auto& imageFile, imageList)
    {
        QImageReader imRead(imageFile.absoluteFilePath());
        emit provideImage(QImage2Mat(imRead.read()));
    }
}
