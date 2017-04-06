#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QThread>

#include "scene.h"

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);
    ~Manager();

signals:

public slots:

private:
    std::shared_ptr<Scene> m_scene;

    QThread m_captureThread;
    QThread m_detectorThread;
};

#endif // MANAGER_H
