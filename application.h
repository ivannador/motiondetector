#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

#include "manager.h"

class Application : public QCoreApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

signals:

public slots:

private:
    Manager* m_manager;

};

#endif // APPLICATION_H
