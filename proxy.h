#pragma once
#include "MainWindow.h"
#include "loginwindow.h"
#include <QString>

class Proxy : public QWidget
{
    Q_OBJECT

public:
    Proxy(MainWindow* wndMain = nullptr);
    ~Proxy();
    void start();

private:
    MainWindow* _wndMain;
    LoginWindow* _wndLogin;
    QString _username;

private slots:
    void rcvConnectionLogin(QString);
    void rcvConnectionMain();
};
