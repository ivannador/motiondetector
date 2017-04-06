#ifndef SCENE_H
#define SCENE_H

#include <QObject>

#include "helpertypes.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();

    struct Object
    {
        double xCoordinate;
        double yCoordinate;
        double velocity;
        double distance;
    };

signals:

public slots:

private:
    QVector<Object> objects;

};

#endif // SCENE_H
