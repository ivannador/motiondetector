#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <QObject>

#include "scene.h"
#include "helpertypes.h"

class ObjectDetector : public QObject
{
    Q_OBJECT
public:
    explicit ObjectDetector(const std::shared_ptr<Scene> scene,
                            QObject *parent = 0);
    ~ObjectDetector();

signals:

public slots:
    void processForeground(const Frame &);

private:
    SBDPtr m_sbd;

    std::shared_ptr<Scene> m_scene;
};

#endif // OBJECTDETECTOR_H
