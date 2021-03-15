#include "threadsend.h"

ThreadSend::ThreadSend(QObject *parent) : QThread(parent)
{
    m_msec = 50;
}
ThreadSend::~ThreadSend()
{

}
void ThreadSend::setmsec(QString msecstr)
{
    bool ok;
    m_msec = msecstr.toInt(&ok,10);
    if(!ok)
        m_msec = 50;
}

void ThreadSend::run()
{
    while(1)
    {
        emit sendba();
        msleep(m_msec);
    }
}
