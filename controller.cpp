#include "controller.h"
Controller::Controller(QObject *parent) : QObject(parent){}
Controller::~Controller()
{
    if (findThread != nullptr)
    {
        if (findThread->isRunning())
        {
            findThread->findStop();

            findThread->terminate();
        }
        findThread.reset();
    }
}

void Controller::startFind(QString dir, QString file)
{
    if (findThread.get())
    {
        findThread->findStop();
        findThread->terminate();
        findThread.reset();
        return;
        }
    findThread = QSharedPointer<ThreadFinder>::create(dir, file);
    findThread->start(QThread::NormalPriority);
    connect(findThread.get(), SIGNAL(stopedThread()), this,SLOT(deleteThread()));
    connect(findThread.get(), SIGNAL(writeFoundPath(QString)), this,SLOT(printCurrentPath(QString)));
    connect(findThread.get(), SIGNAL(findFile(QString)), this,SLOT(genStringPathFile(QString)));
    connect(findThread.get(), SIGNAL(stopedThread()) ,this, SLOT(finishThread()));
    emit newFind();
}

void Controller::deleteThread()
{
    findThread.reset();
}

void Controller::printCurrentPath(QString path)
{
    currentPath = path;
    emit changFindPath(path);
}

void Controller::genStringPathFile(QString file)
{
    QString s = currentPath + file;
    emit genPathOfFile(s);
}

void Controller::finishThread()
{
    emit changFindPath(tr("FINISH"));
}
