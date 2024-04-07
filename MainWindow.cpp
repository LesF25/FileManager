/*
 * добавить контекстное меню при нажатии правой кнопки мыши (delete, закрепить на панели быстрого доступа)
 * сделать так, чтобы высвечивалось изначально окно для входа
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

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    _ltMain = new QVBoxLayout();
    this->setLayout(_ltMain);
    this->setFixedSize(400, 380);

    _wndLogin = new LoginWindow();
    _wndFileContent = new FileContentWindow();
    _wndCreateFile = new CreateFileWindow();
    _wndCreateFolder = new CreateFolderWindow();

    _wndLogin->setWindowModality(Qt::ApplicationModal);
    _wndFileContent->setWindowModality(Qt::ApplicationModal);
    _wndCreateFile->setWindowModality(Qt::ApplicationModal);
    _wndCreateFolder->setWindowModality(Qt::ApplicationModal);

    this->setWindowTitle("File Manager");
    // ================ HEADER LAYOUT ================

    QHBoxLayout* ltHeader = new QHBoxLayout();

    _edCurrentFolder = new QLineEdit(this);
    _edCurrentFolder->setReadOnly(true);
    _edCurrentFolder->setPlaceholderText("Log in to your account.");
    _edCurrentFolder->setFixedHeight(30);

    ltHeader->addWidget(_edCurrentFolder);

    // ================= BODY LAYOUT =================
    QVBoxLayout* ltBody = new QVBoxLayout();

    _listFolderContents = new QListWidget();

    _listQuickAccess = new IMQuickAccess();
    _listQuickAccess->setFixedHeight(95);

    ltBody->addWidget(_listFolderContents);
    ltBody->addWidget(_listQuickAccess);

    // ================ FOOTER LAYOUT ================
    QVBoxLayout* ltFooter = new QVBoxLayout();

    QHBoxLayout* ltFile = new QHBoxLayout();
    _btCreateFile = new QPushButton("Create File", this);
    _btCreateFolder = new QPushButton("Create Folder", this);
    _btDelete = new QPushButton("Delete", this);

    _btCreateFile->setEnabled(false);
    _btCreateFolder->setEnabled(false);
    _btDelete->setEnabled(false);

    QHBoxLayout* ltNavigation = new QHBoxLayout();
    _btExitAccount = new QPushButton("Exit Account", this);
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
    ltNavigation->addWidget(_btExitAccount);
    ltNavigation->addWidget(_btForwardFolder);

    ltFooter->addLayout(ltFile);
    ltFooter->addLayout(ltNavigation);
    // ===============================================

    _ltMain->addLayout(ltHeader);
    _ltMain->addLayout(ltBody);
    _ltMain->addLayout(ltFooter);

    connect(_btCreateFile, &QPushButton::clicked, this, &MainWindow::createFile);
    connect(this, &MainWindow::signCreateFile, _wndCreateFile, &CreateFileWindow::rcvConnect);
    connect(_wndCreateFile, &CreateFileWindow::signCreateFile, this, &MainWindow::displayFile);
    connect(_wndCreateFile, &CreateFileWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_btCreateFolder, &QPushButton::clicked, this, &MainWindow::createFolder);
    connect(this, &MainWindow::signCreateFolder, _wndCreateFolder, &CreateFolderWindow::rcvConnect);
    connect(_wndCreateFolder, &CreateFolderWindow::signCreateFolder, this, &MainWindow::displayFolder);
    connect(_wndCreateFolder, &CreateFolderWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_wndLogin, &LoginWindow::logIn, this, &MainWindow::rcvConnectLogIn);
    connect(_wndLogin, &LoginWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_wndFileContent, &FileContentWindow::fileSaved, this, &MainWindow::rcvConnectSaveFile);
    connect(_wndFileContent, &FileContentWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_btDelete, &QPushButton::clicked, this, &MainWindow::deleteElement);
    connect(_btBackFolder, &QPushButton::clicked, this, &MainWindow::moveBackFolder);
    connect(_btForwardFolder, &QPushButton::clicked, this, &MainWindow::moveForwardFolder);
    connect(_btExitAccount, &QPushButton::clicked, this, &MainWindow::exitAccount);

    connect(_listFolderContents, &QListWidget::itemDoubleClicked, this, &MainWindow::moveFolder);
    connect(_listFolderContents, &QListWidget::itemClicked, this, &MainWindow::clearChoiceQuickList);
    connect(_listQuickAccess, &IMQuickAccess::itemDoubleClicked, this, &MainWindow::quickMoveFodler);
    connect(_listQuickAccess, &IMQuickAccess::signSendPath, this, &MainWindow::rcvConnectQuickAccess);
    connect(_listQuickAccess, &IMQuickAccess::itemClicked, this, &MainWindow::clearChoiceContentList);
    connect(this, &MainWindow::signQuickMoveFolder, _listQuickAccess, &IMQuickAccess::quickMoveFolder);

    connect(this, &MainWindow::sendCurrentFile, _wndFileContent, &FileContentWindow::rcvCurrentFile);
    connect(this, &MainWindow::openFile, this, &MainWindow::changeFile);
}

void MainWindow::setAccessMainWindow()
{
    this->show();
}

void MainWindow::clearChoiceContentList()
{
    _listFolderContents->clearSelection();
}

void MainWindow::clearChoiceQuickList()
{
    _listQuickAccess->clearSelection();
}

void MainWindow::quickMoveFodler()
{
    if (_btBackFolder->isEnabled())
        _btBackFolder->setEnabled(true);

    emit signQuickMoveFolder();
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
    _listQuickAccess->addQuickAccess(_currentPath);

    if (!_btForwardFolder->isEnabled())
        _btForwardFolder->setEnabled(true);

    QString path = _backFolderPath.at(0);
    if (path == _backFolderPath.at(_backFolderPath.length() - 2))
        _btBackFolder->setEnabled(false);

    _listFolderContents->clear();
    _backFolderPath.pop_back();

    _currentPath = _backFolderPath[_backFolderPath.size() - 1];
    _edCurrentFolder->setText(_currentPath);
    displayContent(_currentPath);
}

void MainWindow::moveForwardFolder()
{
    if (!_btBackFolder->isEnabled())
        _btBackFolder->setEnabled(true);

    int it;
    for (it = 0; it < _forwardFolderPath.size(); ++it)
        if (_currentPath == _forwardFolderPath.at(it))
            break;

    ++it;
    QDir dirForward(_forwardFolderPath.at(it));
    if (!dirForward.exists())
    {
        while(_forwardFolderPath.size() != it)
            _forwardFolderPath.pop_back();

        if (_btForwardFolder->isEnabled())
            _btForwardFolder->setEnabled(false);

        QMessageBox::warning(0, "Error", "The directory was not found.");
        return;
    }

    if (it == _forwardFolderPath.size() - 1 && _btForwardFolder->isEnabled())
        _btForwardFolder->setEnabled(false);

    _listFolderContents->clear();

    _currentPath = _forwardFolderPath.at(it);
    _backFolderPath.push_back(_currentPath);
    _edCurrentFolder->setText(_currentPath);

    displayContent(_currentPath);
}

void MainWindow::moveFolder(QListWidgetItem* item)
{
    QString path = _currentPath + item->text();
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

        int it;
        for (it = 0; it < _forwardFolderPath.size(); ++it)
            if (_currentPath == _forwardFolderPath.at(it))
                break;

        while(it < _forwardFolderPath.size() - 1)
            _forwardFolderPath.pop_back();

        _currentPath = path + "/";
        _forwardFolderPath.push_back(_currentPath);
        _backFolderPath.push_back(_currentPath);


        _listFolderContents->clear();
        _edCurrentFolder->setText(_currentPath);
        displayContent(_currentPath);
    }
}

void MainWindow::displayFile(QString path)
{
    _listFolderContents->clearSelection();
    this->close();
    _wndFileContent->show();    
    emit sendCurrentFile(path);
}

void MainWindow::displayFolder(QString folderName)
{
    this->show();
    _listFolderContents->addItem(folderName);
}

void MainWindow::changeFile()
{
    QString path = _currentPath + _listFolderContents->currentItem()->text();
    QFile file(path);
    QFileInfo fileInfo(file);

    if (!file.exists() || !fileInfo.isFile())
    {
        QMessageBox::warning(0, "Error", "File doesn't exists");
        return;
    }

    this->close();
    _wndFileContent->show();
    emit sendCurrentFile(path);
}

void MainWindow::deleteElement()
{
    if (_listFolderContents->currentItem() == nullptr)
    {
        QMessageBox::warning(0, "Error", "Select the item to delete.");
        return;
    }
    QString path = _currentPath + _listFolderContents->currentItem()->text();

    QFile file(path);
    QFileInfo infoFile(file);
    if (file.exists() && infoFile.isFile())
    {
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

void MainWindow::exitAccount()
{
    _edCurrentFolder->clear();
    _currentPath.clear();
    _listFolderContents->clear();

    this->close();
    _wndLogin->show();
}

void MainWindow::rcvConnectQuickAccess(QString path)
{
    // очистка текщуей дирректории

    _listFolderContents->clear();
    _edCurrentFolder->setText(path);

    displayContent(path);
}

void MainWindow::rcvConnectLogIn(QString username)
{
    _username = username;
    _currentPath = "C:/Users/getd8/Desktop/FileManager/users/" + _username + "/";
    _forwardFolderPath.push_back(_currentPath);
    _backFolderPath.push_back(_currentPath);


    _edCurrentFolder->setText(_currentPath);
    QDir dir(_currentPath);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isFile())
            _listFolderContents->addItem(fileInfo.fileName());
        else
            _listFolderContents->addItem(fileInfo.fileName());
    }

    _btCreateFile->setEnabled(true);
    _btCreateFolder->setEnabled(true);
    _btDelete->setEnabled(true);

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

    if (flag)
        _listFolderContents->addItem(fileName);

    this->show();
}

void MainWindow::displayContent(QString path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        _listFolderContents->addItem(fileInfo.fileName());
    }
}

MainWindow::~MainWindow()
{
    delete _wndCreateFile;
    delete _wndCreateFolder;
    delete _wndFileContent;
    delete _wndLogin;
}
