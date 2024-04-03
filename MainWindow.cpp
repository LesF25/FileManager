/*
    прокси может хранить бд и сам проверять доступ пользователя, кэшировать переходы пользователя
    сделать так, чтобы вылетали окна с созданием папок/файлов, но при этом оставалось главноее окно и к нему был закрыт доступ
*/

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QFrame>
#include <QWidgetItem>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QString username, QWidget *parent)
    : QWidget(parent), _username(username)
{
    _currentPath.push_back("C:/Users/getd8/Desktop/FileManager/users/" + _username + "/");
    _forwardFolderPath.push_back("C:/Users/getd8/Desktop/FileManager/users/" + _username + "/");

    // ==============================================
    _ltMain = new QVBoxLayout();
    this->setLayout(_ltMain);
    this->setFixedSize(400, 350);

    _wndFileContent = new FileContentWindow();
    _wndCreateFile = new CreateFileWindow();
    _wndCreateFolder = new CreateFolderWindow();

    this->setWindowTitle("File Manager");
    // ================ HEADER LAYOUT ================

    QHBoxLayout* ltHeader = new QHBoxLayout();

    _edCurrentFolder = new QLineEdit(this);
    _edCurrentFolder->setReadOnly(true);
    _edCurrentFolder->setPlaceholderText("Log in to your account.");
    _edCurrentFolder->setFixedHeight(30);

    ltHeader->addWidget(_edCurrentFolder);

    // ================= BODY LAYOUT =================
    QHBoxLayout* ltBody = new QHBoxLayout();
    _listFolderContents = new QListWidget();
    _listFolderContents->setFixedHeight(220);

    ltBody->addWidget(_listFolderContents);

    // ================ FOOTER LAYOUT ================
    QVBoxLayout* ltFooter = new QVBoxLayout();

    QHBoxLayout* ltFile = new QHBoxLayout();
    _btCreateFile = new QPushButton("Create File", this);
    _btCreateFolder = new QPushButton("Create Folder", this);
    _btDelete = new QPushButton("Delete", this);

    QHBoxLayout* ltNavigation = new QHBoxLayout();
    _btBackFolder = new QPushButton(this);
    _btBackFolder->setIcon(QIcon(":/resource/icons/arrowBack.png"));
    _btForwardFolder = new QPushButton(this);
    _btForwardFolder->setIcon(QIcon(":/resource/icons/arrowForward.png"));

    _btBackFolder->setEnabled(false);
    _btForwardFolder->setEnabled(false);

    ltFile->addWidget(_btCreateFile);
    ltFile->addWidget(_btDelete);
    ltFile->addWidget(_btCreateFolder);

    ltNavigation->addWidget(_btBackFolder);
    ltNavigation->addWidget(_btForwardFolder);

    ltFooter->addLayout(ltFile);
    ltFooter->addLayout(ltNavigation);
    // ===============================================

    _ltMain->addLayout(ltHeader);
    _ltMain->addLayout(ltBody);
    _ltMain->addLayout(ltFooter);

    displayContent();

    connect(_btCreateFile, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(this, &MainWindow::signCreateFile, _wndCreateFile, &CreateFileWindow::rcvConnect);
    connect(_wndCreateFile, &CreateFileWindow::signCancel, this, &MainWindow::rcvConnectCloseFile);
    connect(_wndCreateFile, &CreateFileWindow::signCreateFile, this, &MainWindow::displayFile);

    connect(_btCreateFolder, &QPushButton::clicked, this, &MainWindow::createFolder);
    connect(this, &MainWindow::signCreateFolder, _wndCreateFolder, &CreateFolderWindow::rcvConnect);
    connect(_wndCreateFolder, &CreateFolderWindow::signCancel, this, &MainWindow::rcvConnectCloseFile);
    connect(_wndCreateFolder, &CreateFolderWindow::signCreateFolder, this, &MainWindow::displayFolder);

    connect(_btDelete, &QPushButton::clicked, this, &MainWindow::deleteElement);
    connect(_listFolderContents, &QListWidget::itemDoubleClicked, this, &MainWindow::moveFolder);
    connect(_btBackFolder, &QPushButton::clicked, this, &MainWindow::moveBackFolder);
    connect(_btForwardFolder, &QPushButton::clicked, this, &MainWindow::moveForwardFolder);

    connect(_wndFileContent, &FileContentWindow::fileSaved, this, &MainWindow::rcvConnectSaveFile);
    connect(_wndFileContent, &FileContentWindow::fileClose, this, &MainWindow::rcvConnectCloseFile);
    connect(this, &MainWindow::sendCurrentFile, _wndFileContent, &FileContentWindow::rcvCurrentFile);
    connect(this, &MainWindow::openFile, this, &MainWindow::changeFile);
}

void MainWindow::displayContent()
{
    QString path = getFullPath();

    _edCurrentFolder->setText(path);
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isFile())
            _listFolderContents->addItem(fileInfo.fileName());
        else
            _listFolderContents->addItem(fileInfo.fileName());
    }
}

void MainWindow::createFile()
{
    this->close();
    _wndCreateFile->show();

    emit signCreateFile(_edCurrentFolder->text());
}

void MainWindow::createFolder()
{
    this->close();
    _wndCreateFolder->show();

    emit signCreateFolder(_edCurrentFolder->text());
}

void MainWindow::moveBackFolder()
{
    if (!_btForwardFolder->isEnabled())
        _btForwardFolder->setEnabled(true);

    if (_edCurrentFolder->text() == ("C:/Users/getd8/Desktop/FileManager/users/" + _username + "/" + _currentPath[_currentPath.length() - 1]))
        _btBackFolder->setEnabled(false);


    _listFolderContents->clear();
    _currentPath.pop_back();
    QString path = getFullPath();
    _edCurrentFolder->setText(path);

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        _listFolderContents->addItem(fileInfo.fileName());
    }
}

void MainWindow::moveForwardFolder()
{
    QString pathForward;
    for (int i = 0; i < _forwardFolderPath.size(); i++)
        pathForward += _forwardFolderPath[i];

    QDir dirForward(pathForward);
    if (!dirForward.exists())
    {
        int i = 0;
        pathForward = _forwardFolderPath[i];
        dirForward.setPath(pathForward);
        while (dirForward.exists())
        {
            ++i;
            pathForward += _forwardFolderPath[i];
            dirForward.setPath(pathForward);
        }

        while(_forwardFolderPath.size() != i)
            _forwardFolderPath.pop_back();

        QMessageBox::warning(0, "Error", "The directory was not found.");
        return;
    }

    if (!_btBackFolder->isEnabled())
        _btBackFolder->setEnabled(true);

    int i = _currentPath.size();
    if (i == _forwardFolderPath.size() - 1 && _btForwardFolder->isEnabled())
        _btForwardFolder->setEnabled(false);

    if ( i != _forwardFolderPath.size())
        _currentPath.push_back(_forwardFolderPath.at(i));

    _listFolderContents->clear();

    QString path = getFullPath();
    _edCurrentFolder->setText(path);

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        _listFolderContents->addItem(fileInfo.fileName());
    }
}

void MainWindow::moveFolder(QListWidgetItem* item)
{
    QString path = getFullPath() + item->text();
    QFile file(path);
    QFileInfo fileInfo(file);

    if(file.exists() && fileInfo.isFile())
        emit openFile();
    else if (file.exists() && fileInfo.isDir())
    {
        if (!_btBackFolder->isEnabled())
            _btBackFolder->setEnabled(true);

        if (_btForwardFolder->isEnabled())
            _btForwardFolder->setEnabled(false);

        int i = _currentPath.size();
        while(i != _forwardFolderPath.size())
            _forwardFolderPath.pop_back();

        _forwardFolderPath.push_back(item->text() + "/");

        _currentPath.push_back(item->text() + "/");
        _listFolderContents->clear();

        QString path = getFullPath();
        _edCurrentFolder->setText(path);

        QDir dir(path);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 2; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            _listFolderContents->addItem(fileInfo.fileName());
        }
    }
}

void MainWindow::displayFile(QString path)
{
    _listFolderContents->clearSelection();
    _wndFileContent->show();
    emit sendCurrentFile(path);
}

void MainWindow::displayFolder(QString folderName)
{
    _listFolderContents->addItem(folderName);
    emit signLoggingCreateFolder(getFullPath() + folderName + "/");
    this->show();
}

void MainWindow::changeFile()
{
    QString path = getFullPath()+ _listFolderContents->currentItem()->text();
    QFile file(path);
    QFileInfo fileInfo(file);

    if (!file.exists() || !fileInfo.isFile())
    {
        QMessageBox::warning(0, "Error", "File doesn't exists");
        return;
    }

    _wndFileContent->show();
    this->close();
    emit sendCurrentFile(path);
}

void MainWindow::deleteElement()
{
    if (_listFolderContents->currentItem() == nullptr)
    {
        QMessageBox::warning(0, "Error", "Select the item to delete.");
        return;
    }
    QString path = getFullPath() + _listFolderContents->currentItem()->text();

    QFile file(path);
    QFileInfo infoFile(file);
    if (file.exists() && infoFile.isFile())
    {
        emit signLoggingDeleteElement(path);
        file.remove();
        QListWidgetItem *item = _listFolderContents->item(_listFolderContents->currentRow());
        if (!item)
        {
            QMessageBox::information(this, "Error", "Item in the list is not selected");
            return;
        }
        _listFolderContents->takeItem(_listFolderContents->currentRow());
        return;
    }

    QDir dir(path);
    QFileInfo infoDir(path);
    if (dir.exists() && infoDir.isDir())
    {
        emit signLoggingDeleteElement(path + "/");

        dir.removeRecursively();
        QListWidgetItem *item = _listFolderContents->item(_listFolderContents->currentRow());
        if (!item)
        {
            QMessageBox::information(this, "Error", "Item in the list is not selected");
            return;
        }
        _listFolderContents->takeItem(_listFolderContents->currentRow());

        return;
    }

    QMessageBox::warning(0, "Error", "File doesn't exists");
    return;
}

void MainWindow::rcvConnectCloseFile()
{
    this->show();
}

void MainWindow::rcvConnectSaveFile(QString fileName)
{
    bool flag = true;
    QListWidgetItem* item;
    for (int i = 0; i < _listFolderContents->count(); i++)
    {
        item = _listFolderContents->item(i);
        if (item->text() == fileName)
            flag = false;
    }

    QString path = getFullPath();
    if (flag)
    {
        _listFolderContents->addItem(fileName);
        emit signLoggingCreateFile(path + fileName);
    }
    else
        emit signLoggingChangeFile(path + fileName);

    this->show();
}

QString MainWindow::getFullPath()
{
    QString path;
    for(int i = 0; i < _currentPath.size(); i++)
        path += _currentPath.at(i);

    return path;
}

MainWindow::~MainWindow()
{
    delete _wndCreateFile;
    delete _wndCreateFolder;
    delete _wndFileContent;
}
