#ifndef TESTIMAGEPROVIDER_H
#define TESTIMAGEPROVIDER_H

#include <QObject>
#include <QImageReader>

#include "helpertypes.h"

class TestImageProvider : public QObject
{
    Q_OBJECT
public:
    explicit TestImageProvider(QObject *parent = 0)
        : QObject(parent)
    {}

public slots:
    void parseImages();

signals:
    void provideImage(const Frame &);
};

#endif // TESTIMAGEPROVIDER_H
