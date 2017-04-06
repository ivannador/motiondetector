#ifndef BGSUBTRACTOR_H
#define BGSUBTRACTOR_H

#include <QObject>

#include "helpertypes.h"

class BGSubtractor : public QObject
{
    Q_OBJECT
public:
    explicit BGSubtractor(QObject *parent = 0);
    ~BGSubtractor();

signals:
    void sendForeground(const Frame &);

public slots:
    void processImage(const Frame &);

private:
    BGSPtr m_bgs;

};

#endif // BGSUBTRACTOR_H
