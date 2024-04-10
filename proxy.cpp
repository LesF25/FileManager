#include "proxy.h"
#include <QDir>
#include <QTextStream>
#include <QDateTime>

Proxy::Proxy(MainWindow* wndMain) :
    _wndMain(wndMain)
{
    _wndLogin = new LoginWindow();
    connect(_wndLogin, &LoginWindow::logIn, this, &Proxy::rcvConnectionLogin);

    if (_wndMain != nullptr)
        connect(_wndMain, &MainWindow::signExitAccount, this, &Proxy::rcvConnectionMain);
}

void Proxy::start()
{
    _wndLogin->show();
}

void Proxy::rcvConnectionLogin(QString username)
{
    if (_wndMain == nullptr)
    {
        _username = username;
        _wndMain = new MainWindow(username, nullptr);
        connect(_wndMain, &MainWindow::signExitAccount, this, &Proxy::rcvConnectionMain);
    }

    _wndMain->show();
}

void Proxy::rcvConnectionMain()
{
    delete _wndMain;
    _wndMain = nullptr;

    _wndLogin->show();
}

Proxy::~Proxy()
{
    delete _wndLogin;
    delete _wndMain;
}
