#pragma once
#include "MainWindow.h"
#include "loginwindow.h"
#include <QString>

class IService
{
public:
    virtual void start() = 0;
};

class Service : public QWidget, public IService
{
    Q_OBJECT
public:
    Service(QString);
    ~Service();
    void start() override;

private:
    MainWindow* _wndMain;

signals:
    void signCreateFolder(QString);
    void signCreateFile(QString);
    void signChangeFile(QString);
    void signDeleteElement(QString);

public slots:
    void createFolder(QString);
    void createFile(QString);
    void changeFile(QString);
    void deleteElement(QString);
};

class Proxy : public QWidget, public IService
{
    Q_OBJECT
public:
    Proxy(Service*);
    ~Proxy();
    void start() override;

private:
    Service* _service;
    LoginWindow* _wndLogin;
    QString _username;
    QString _loggingPath;
    QFile _fileLogging;

private slots:
    void rcvConnection(QString);
    void createFolder(QString);
    void createFile(QString);
    void changeFile(QString);
    void deleteElement(QString);
};
