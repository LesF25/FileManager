#include "proxy.h"
#include <QDir>
#include <QTextStream>
#include <QDateTime>

Service::Service(QString username)
{
    _wndMain = new MainWindow(username, nullptr);

    connect(_wndMain, &MainWindow::signLoggingCreateFile, this, &Service::createFile);
    connect(_wndMain, &MainWindow::signLoggingCreateFolder, this, &Service::createFolder);
    connect(_wndMain, &MainWindow::signLoggingChangeFile, this, &Service::changeFile);
    connect(_wndMain, &MainWindow::signLoggingDeleteElement, this, &Service::deleteElement);
}

void Service::createFile(QString path) { emit signCreateFile(path); }
void Service::createFolder(QString path) { emit signCreateFolder(path); }
void Service::changeFile(QString path) { emit signChangeFile(path); }
void Service::deleteElement(QString path) { emit signDeleteElement(path); }

void Service::start()
{
    _wndMain->show();
}

Service::~Service()
{
    delete _wndMain;
}

// =========================================================

Proxy::Proxy(Service* service) :
    _service(service)
{
    _wndLogin = new LoginWindow();
    _loggingPath = "C:/Users/getd8/Desktop/FileManager/logging/";

    QDir dir(_loggingPath);
    if (!dir.exists())
        dir.mkdir(_loggingPath);

    connect(_wndLogin, &LoginWindow::logIn, this, &Proxy::rcvConnection);
}

void Proxy::start()
{
    _wndLogin->show();
}

void Proxy::deleteElement(QString path)
{
    QFileInfo fileInfo(_fileLogging);

    if(_fileLogging.exists() && fileInfo.isFile())
    {
        bool openFile = _fileLogging.open(QIODevice::Append | QIODevice::WriteOnly);
        if (openFile)
        {
            QTextStream textStream(&_fileLogging);
            QString time = QDateTime::currentDateTime().toString();
            textStream << QString("%1\nDELETED the element in the PATH: %2.\n\n").arg(time, path);

            _fileLogging.close();
        }
    }
}

void Proxy::createFolder(QString path)
{
    QFileInfo fileInfo(_fileLogging);

    if(_fileLogging.exists() && fileInfo.isFile())
    {
        bool openFile = _fileLogging.open(QIODevice::Append | QIODevice::WriteOnly);
        if (openFile)
        {
            QTextStream textStream(&_fileLogging);
            QString time = QDateTime::currentDateTime().toString();
            textStream << QString("%1\nCREATED FOLDER in the PATH: %2.\n\n").arg(time, path);

            _fileLogging.close();
        }
    }
}

void Proxy::createFile(QString path)
{
    QFileInfo fileInfo(_fileLogging);

    if(_fileLogging.exists() && fileInfo.isFile())
    {
        bool openFile = _fileLogging.open(QIODevice::Append | QIODevice::WriteOnly);
        if (openFile)
        {
            QTextStream textStream(&_fileLogging);
            QString time = QDateTime::currentDateTime().toString();
            textStream << QString("%1\nCREATED FILE in the PATH: %2.\n\n").arg(time, path);

            _fileLogging.close();
        }
    }
}

void Proxy::changeFile(QString path)
{
    QFileInfo fileInfo(_fileLogging);

    if(_fileLogging.exists() && fileInfo.isFile())
    {
        bool openFile = _fileLogging.open(QIODevice::Append | QIODevice::WriteOnly);
        if (openFile)
        {
            QTextStream textStream(&_fileLogging);
            QString time = QDateTime::currentDateTime().toString();
            textStream << QString("%1\nCHANGED FILE in the PATH: %2.\n\n").arg(time, path);

            _fileLogging.close();
        }
    }
}

void Proxy::rcvConnection(QString username)
{
    _username = username;
    _fileLogging.setFileName(_loggingPath + _username + ".txt");
    bool openFile = _fileLogging.open(QIODevice::Append | QIODevice::WriteOnly);
    if (openFile)
    {
        QTextStream textStream(&_fileLogging);
        QString time = QDateTime::currentDateTime().toString();
        textStream << QString("\n\n=======================================================================\n");
        textStream << QString("%1\nThe user is LOGGED in.\n\n").arg(time);

        _fileLogging.close();
    }

    if (_service == nullptr)
        _service = new Service(_username);

    connect(_service, &Service::signCreateFile, this, &Proxy::createFile);
    connect(_service, &Service::signCreateFolder, this, &Proxy::createFolder);
    connect(_service, &Service::signDeleteElement, this, &Proxy::deleteElement);
    connect(_service, &Service::signChangeFile, this, &Proxy::changeFile);

    _service->start();
}

Proxy::~Proxy()
{
    delete _wndLogin;
}
