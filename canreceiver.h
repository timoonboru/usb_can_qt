#ifndef CANRECEIVER_H
#define CANRECEIVER_H

#include <QThread>
#include "ControlCAN.h"
#include "resource.h"

class CanReceiver : public QThread
{
    Q_OBJECT

signals:
    void message(int id, float dist, float angel, float velocity, QString hex);
public:
    CanReceiver(int DevType , int DevIndex );
    ~CanReceiver();

    void run();

private:
    int m_DevType;
    int m_DevIndex;

};

#endif // CANRECEIVER_H
