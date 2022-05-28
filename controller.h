#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "threadfinder.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    void startFind(QString dir, QString file);
private:
    QSharedPointer<ThreadFinder>findThread;
private:
    QString currentPath;
signals:
    void changFindPath(QString);
    void genPathOfFile(QString);
    void newFind();
public slots:
    void deleteThread();
    void printCurrentPath(QString);
    void genStringPathFile(QString);
    void finishThread();
};

#endif // CONTROLLER_H
