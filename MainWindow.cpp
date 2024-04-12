/* заменить QListWidget быстрого доступа на сворачиваемый список, а QListWidget на виджет похожий на таблицу, содержащий несколько областей по типу дата создания, размер и т.д.
 * Выделение множества объектов в QListWidget
 * загрузка файлов
 * открывать картинки

 * Темная/светлая тема
 * Работа с файлами Microsoft Office
 * Проверка почты
*/

#include "MainWindow.h"
// #include <private/qzipreader_p.h>
// #include <private/qzipwriter_p.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QFrame>
#include <QWidgetItem>

#include <QFileInfoList>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMenu>

MainWindow::MainWindow(QString username, QWidget *parent)
    : QWidget(parent)
{
    _ltMain = new QVBoxLayout();
    this->setLayout(_ltMain);
    this->setFixedSize(500, 380);
    this->setWindowIcon(QIcon(":resource/icons/FileManager.png"));
    this->setWindowTitle("File Manager");

    _wndFileContent = new FileContentWindow();
    _wndCreateFile = new CreateFileWindow();
    _wndCreateFolder = new CreateFolderWindow();
    _wndRenameFile = new RenameFileWindow();
    _wndRenameFolder = new RenameFolderWindow();

    _username = username;
    _currentPath = "C:/Users/getd8/Desktop/FileManager/users/" + _username + "/";
    _forwardFolderPath.push_back(_currentPath);
    _backFolderPath.push_back(_currentPath);
// ================ Панель инструментов ================
    QWidget* spacer1 = new QWidget();
    QWidget* spacer2 = new QWidget();

    spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _actBackFolder = new QAction(this);
    _actForwardFolder = new QAction(this);
    _actCreateFolder = new QAction(this);
    _actCreateFile = new QAction(this);
    _actDelete = new QAction(this);
    _actExitAccount = new QAction(this);
    _actDownload = new QAction(this);

    _actBackFolder->setIcon(QIcon(":resource/icons/arrowBack.png"));
    _actForwardFolder->setIcon(QIcon(":resource/icons/arrowForward.png"));
    _actCreateFolder->setIcon(QIcon(":resource/icons/createFolder.png"));
    _actCreateFile->setIcon(QIcon(":resource/icons/createFile.png"));
    _actDelete->setIcon(QIcon(":resource/icons/delete.png"));
    _actDownload->setIcon(QIcon(":resource/icons/download.png"));
    _actExitAccount->setIcon(QIcon(":resource/icons/exitAccount.png"));

    _actBackFolder->setEnabled(false);
    _actForwardFolder->setEnabled(false);
    _actDownload->setEnabled(false);

    _actBackFolder->setToolTip("Move Back");
    _actForwardFolder->setToolTip("Move Forward");
    _actCreateFolder->setToolTip("Create Folder");
    _actCreateFile->setToolTip("Create File");
    _actDelete->setToolTip("Delete");
    _actExitAccount->setToolTip("Exit Account");
    _actDownload->setToolTip("Download");

    QAction* empty = new QAction(this);
    empty->setEnabled(false);

    _toolBar = new QToolBar(this);
    _toolBar->addAction(_actBackFolder);
    _toolBar->addAction(_actForwardFolder);
    _toolBar->addWidget(spacer1);
    _toolBar->addAction(_actCreateFolder);
    _toolBar->addAction(_actCreateFile);
    _toolBar->addAction(_actDelete);
    _toolBar->addAction(_actDownload);
    _toolBar->addWidget(spacer2);
    _toolBar->addAction(empty);
    _toolBar->addAction(_actExitAccount);

// ================= Context Menu ==================
    _actInsert = new QAction("Insert", this);
    _actInsert->setEnabled(false);

// ================ Верхний уровень ================
    QHBoxLayout* ltTop = new QHBoxLayout();

    _edCurrentFolder = new QLineEdit(this);
    _edCurrentFolder->setReadOnly(true);
    _edCurrentFolder->setPlaceholderText("Log in to your account.");
    _edCurrentFolder->setFixedHeight(30);
    _edCurrentFolder->setText(_currentPath);

    ltTop->addWidget(_edCurrentFolder);

// ================= BODY LAYOUT ===================
    QVBoxLayout* ltAverage = new QVBoxLayout();
    QHBoxLayout* ltList = new QHBoxLayout();

    _listFolderContents = new QListWidget();
    _listFolderContents->setContextMenuPolicy(Qt::CustomContextMenu);

    _listQuickAccess = new IMQuickAccess();
    _listQuickAccess->setMaximumWidth(120);
    _listQuickAccess->setContextMenuPolicy(Qt::CustomContextMenu);

    ltList->addWidget(_listQuickAccess);
    ltList->addWidget(_listFolderContents);
    ltAverage->addLayout(ltList);
//    ltAverage->addWidget(_listFolderContents);
//    ltAverage->addWidget(_listQuickAccess);

// ===============================================
    _ltMain->addLayout(ltTop);
    _ltMain->addLayout(ltAverage);
    _ltMain->addWidget(_toolBar);
    displayContent(_currentPath);

// ===============================================

    connect(_actInsert, &QAction::triggered, this, &MainWindow::insertElement);

    connect(_actCreateFile, &QAction::triggered, this, &MainWindow::createFile);
    connect(this, &MainWindow::signCreateFile, _wndCreateFile, &CreateFileWindow::rcvConnect);
    connect(_wndCreateFile, &CreateFileWindow::signCreateFile, this, &MainWindow::displayContentFile);
    connect(_wndCreateFile, &CreateFileWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_actCreateFolder, &QAction::triggered, this, &MainWindow::createFolder);
    connect(this, &MainWindow::signCreateFolder, _wndCreateFolder, &CreateFolderWindow::rcvConnect);
    connect(_wndCreateFolder, &CreateFolderWindow::signCreateFolder, this, &MainWindow::displayFolder);
    connect(_wndCreateFolder, &CreateFolderWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_actBackFolder, &QAction::triggered, this, &MainWindow::moveBackFolder);
    connect(_actForwardFolder, &QAction::triggered, this, &MainWindow::moveForwardFolder);
    connect(_actDelete, &QAction::triggered, this, &MainWindow::deleteElement);
    connect(_actExitAccount, &QAction::triggered, this, &MainWindow::exitAccount);

    connect(_wndFileContent, &FileContentWindow::fileSaved, this, &MainWindow::rcvConnectSaveFile);
    connect(_wndFileContent, &FileContentWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);
    connect(this, &MainWindow::signSendCurrentFile, _wndFileContent, &FileContentWindow::rcvCurrentFile);

    connect(this, &MainWindow::signRenameFile, _wndRenameFile, &RenameFileWindow::rcvConnect);
    connect(_wndRenameFile, &RenameFileWindow::signRenameFile, this, &MainWindow::rcvConnectRename);
    connect(_wndRenameFile, &RenameFileWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(this, &MainWindow::signRenameFolder, _wndRenameFolder, &RenameFolderWindow::rcvConnect);
    connect(_wndRenameFolder, &RenameFolderWindow::signRenameFolder, this, &MainWindow::rcvConnectRename);
    connect(_wndRenameFolder, &RenameFolderWindow::signCloseWindow, this, &MainWindow::setAccessMainWindow);

    connect(_listFolderContents, &QListWidget::itemDoubleClicked, this, &MainWindow::moveFolder);
    connect(_listFolderContents, &QListWidget::itemClicked, this, &MainWindow::clearChoiceQuickList);
    connect(_listFolderContents, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenuContentList);

    connect(_listQuickAccess, &IMQuickAccess::itemDoubleClicked, this, &MainWindow::quickMoveFolder);
    connect(this, &MainWindow::signQuickMoveFolder, _listQuickAccess, &IMQuickAccess::quickMoveFolder);
    connect(_listQuickAccess, &IMQuickAccess::signSendPath, this, &MainWindow::rcvConnectQuickAccess);
    connect(_listQuickAccess, &IMQuickAccess::itemClicked, this, &MainWindow::clearChoiceContentList);
    connect(_listQuickAccess, &IMQuickAccess::customContextMenuRequested, this, &MainWindow::showContextMenuQuickList);

    connect(this, &MainWindow::signOpenFile, this, &MainWindow::changeFile);
}

void MainWindow::rcvConnectRename(QString newElementName)
{
    QFile file(_currentPath + _listFolderContents->currentItem()->text());
    QFileInfo fileInfo(file);

    if (fileInfo.exists() && fileInfo.isFile())
    {
        int check = _listQuickAccess->getNumRow(_beforeRenameFile);
        if (check != -1)
            _listQuickAccess->removeQuickAccess(check);
        _beforeRenameFile.clear();
        file.rename(newElementName);

        _listQuickAccess->addQuickAccess(newElementName);
        _listFolderContents->clear();
        displayContent(_currentPath);
        this->show();

        return;
    }

    if(fileInfo.exists() && fileInfo.isDir())
    {
        int check = _listQuickAccess->getNumRow(_beforeRenameFile);
        if (check != -1)
            _listQuickAccess->removeQuickAccess(check);
        _beforeRenameFile.clear();

        QDir dir(_currentPath + _listFolderContents->currentItem()->text() + "/");
        dir.rename(dir.absolutePath(), newElementName);

        _listQuickAccess->addQuickAccess(newElementName);
        _listFolderContents->clear();
        displayContent(_currentPath);
        this->show();

        return;
    }

    QMessageBox::warning(0, "Error", "Element couldn't be renamed.");
    return;
}

void MainWindow::renameElement()
{
    QString path = _currentPath + _listFolderContents->currentItem()->text();
    QFile file(path);
    QFileInfo fileInfo(file);
    if (fileInfo.exists() && fileInfo.isFile())
    {
        _beforeRenameFile = path;
        this->close();
        _wndRenameFile->show();

        emit signRenameFile(_currentPath + _listFolderContents->currentItem()->text());
        return;
    }

    if (fileInfo.exists() && fileInfo.isDir())
    {
        _beforeRenameFile = path + "/";

        this->close();
        _wndRenameFolder->show();

        emit signRenameFolder(path + "/");
        return;
    }

    QMessageBox::warning(0, "Error", "Element couldn't be renamed.");
    return;
}

void MainWindow::copyElement()
{
    _copyPath = _currentPath + _listFolderContents->currentItem()->text();
    if (!_actInsert->isEnabled())
        _actInsert->setEnabled(true);
}

void MainWindow::insertElement()
{
    QFile file(_copyPath);
    QFileInfo fileInfo(file);

    if (file.exists() && fileInfo.isFile())
    {
        int i = 1;
        QString path = _currentPath + fileInfo.baseName() + " - copy." + fileInfo.suffix();
        QFile file(path);
        while (file.exists())
        {
            path = QString(_currentPath + fileInfo.baseName() + " - copy(%1)." + fileInfo.suffix()).arg(i);
            file.setFileName(path);
            ++i;
        }

        bool copy = file.copy(_copyPath, path);
        if (!copy)
        {
            QMessageBox::warning(0, "Error", "File couldn't be copied.");
            return;
        }

        _listFolderContents->clear();
        displayContent(_currentPath);

        return;
    }

    if (fileInfo.exists() && fileInfo.isDir())
    {
        int i = 1;
        QString path = _currentPath + fileInfo.fileName() + " - copy";
        QDir dir(path);
        while (dir.exists())
        {
            path = QString(_currentPath + fileInfo.fileName() + " - copy(%1)").arg(i);
            dir.setPath(path);
            ++i;
        }

        bool copy = copyDir(_copyPath, dir.absolutePath());
        if (!copy)
        {
            QMessageBox::warning(0, "Error", "Folder couldn't be copied.");
            return;
        }

        _listFolderContents->clear();
        displayContent(_currentPath);

        return;
    }

    QMessageBox::warning(0, "Error", "The item could not be copied.");
    return;
}

void MainWindow::fixFolder()
{
    QString path = _currentPath + _listFolderContents->currentItem()->text();
    QFile file(path);
    QFileInfo fileInfo(file);

    if(file.exists() && fileInfo.isFile())
        _listQuickAccess->addQuickAccess(path);
    else if (file.exists() && fileInfo.isDir())
        _listQuickAccess->addQuickAccess(path + "/");
}

void MainWindow::detachFolder()
{
    _listQuickAccess->removeQuickAccess(_listQuickAccess->currentRow());
}

void MainWindow::showContextMenuQuickList(const QPoint& pos)
{
    QListWidgetItem *item = _listQuickAccess->itemAt(pos);
    if (item == nullptr)
        return;

    QMenu menu;
    menu.addAction("Detach", this, &MainWindow::detachFolder);
    menu.exec(_listQuickAccess->mapToGlobal(pos));
}

void MainWindow::showContextMenuContentList(const QPoint& pos)
{
    QListWidgetItem *item = _listFolderContents->itemAt(pos);

    if (item == nullptr)
    {
        QMenu menu;
        menu.addAction("Create folder", this, &MainWindow::createFolder);
        menu.addAction("Create file", this, &MainWindow::createFile);
        menu.addAction(_actInsert);
        menu.exec(_listFolderContents->mapToGlobal(pos));
    }
    else
    {
        QMenu menu;
        menu.addAction("Rename", this, &MainWindow::renameElement);
        menu.addAction("Copy", this, &MainWindow::copyElement);
        menu.addAction("Fix", this, &MainWindow::fixFolder);
        menu.addAction("Delete", this, &MainWindow::deleteElement);
        menu.exec(_listFolderContents->mapToGlobal(pos));
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

void MainWindow::clearChoiceContentList() {
    _listFolderContents->clearSelection();
}

void MainWindow::clearChoiceQuickList() {
    _listQuickAccess->clearSelection();
}

void MainWindow::quickMoveFolder() {
    emit signQuickMoveFolder(_listQuickAccess->currentRow());
}

void MainWindow::moveBackFolder()
{
    if (!_actForwardFolder->isEnabled())
        _actForwardFolder->setEnabled(true);

    QString path = _backFolderPath.at(0);
    if (path == _backFolderPath.at(_backFolderPath.length() - 2))
        _actBackFolder->setEnabled(false);

    _listFolderContents->clear();
    _backFolderPath.pop_back();

    _currentPath = _backFolderPath[_backFolderPath.size() - 1];
    _edCurrentFolder->setText(_currentPath);
    displayContent(_currentPath);
}

void MainWindow::moveForwardFolder()
{
    int it = _forwardFolderPath.indexOf(_currentPath);

    ++it;
    QDir dirForward(_forwardFolderPath.at(it));
    if (!dirForward.exists())
    {
        while(_forwardFolderPath.size() != it)
            _forwardFolderPath.pop_back();

        if (_actForwardFolder->isEnabled())
            _actForwardFolder->setEnabled(false);

        QMessageBox::warning(0, "Error", "The directory was not found.");
        return;
    }

    if (!_actBackFolder->isEnabled())
        _actBackFolder->setEnabled(true);

    if (it == _forwardFolderPath.size() - 1 && _actForwardFolder->isEnabled())
        _actForwardFolder->setEnabled(false);

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
        emit signOpenFile();
    else if (file.exists() && fileInfo.isDir())
    {
        if (!_actBackFolder->isEnabled())
            _actBackFolder->setEnabled(true);

        if (_actForwardFolder->isEnabled())
            _actForwardFolder->setEnabled(false);

        int it = _forwardFolderPath.indexOf(_currentPath);

        if (it != -1)
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

void MainWindow::displayContentFile(QString path)
{
    _listFolderContents->clearSelection();
    this->close();
    _wndFileContent->show();
    emit signSendCurrentFile(path);
}

void MainWindow::displayFolder(QString folderName)
{
    this->show();
    _listFolderContents->addItem(new QListWidgetItem(QIcon(":resource/icons/folder.png"), folderName));
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
    emit signSendCurrentFile(path);
}

void MainWindow::deleteElement()
{
    if (_listFolderContents->currentItem() == nullptr || !_listFolderContents->currentItem()->isSelected())
    {
        QMessageBox::warning(0, "Error", "The item could not be deleted.");
        return;
    }
    QString path = _currentPath + _listFolderContents->currentItem()->text();

    QFile file(path);
    QFileInfo infoFile(file);
    if (file.exists() && infoFile.isFile())
    {
        int check = _listQuickAccess->getNumRow(path);
        if (check != -1)
            _listQuickAccess->removeQuickAccess(check);

        file.remove();

        QListWidgetItem *item = _listFolderContents->item(_listFolderContents->currentRow());
        if (!item)
        {
            QMessageBox::information(this, "Error", "Item in the list is not selected");
            return;
        }
        _listFolderContents->takeItem(_listFolderContents->currentRow());
        _listFolderContents->clearSelection();

        return;
    }

    path += "/";
    QDir dir(path);
    QFileInfo infoDir(path);
    if (dir.exists() && infoDir.isDir())
    {
        int check = _listQuickAccess->getNumRow(path);
        if (check != -1)
            _listQuickAccess->removeQuickAccess(check);

        dir.removeRecursively();
        QListWidgetItem *item = _listFolderContents->item(_listFolderContents->currentRow());
        if (!item)
        {
            QMessageBox::information(this, "Error", "Item in the list is not selected");
            return;
        }
        _listFolderContents->takeItem(_listFolderContents->currentRow());
        _listFolderContents->clearSelection();

        return;
    }

    QMessageBox::warning(0, "Error", "File doesn't exists");
    return;
}

void MainWindow::exitAccount()
{
    _currentPath.clear();
    _edCurrentFolder->clear();
    _forwardFolderPath.clear();
    _backFolderPath.clear();
    _listFolderContents->clear();
    _listQuickAccess->clear();

    this->close();
    emit signExitAccount();
}

void MainWindow::rcvConnectQuickAccess(QString path)
{
    QFile file(path);
    QFileInfo fileInfo(file);

    if(file.exists() && fileInfo.isFile())
    {
        this->close();
        _wndFileContent->show();
        emit signSendCurrentFile(path);
    }
    else if (file.exists() && fileInfo.isDir())
    {
        if (!_actBackFolder->isEnabled())
            _actBackFolder->setEnabled(true);

        if (_actForwardFolder->isEnabled())
            _actForwardFolder->setEnabled(false);

        int itForward = _forwardFolderPath.indexOf(_currentPath);
        if (itForward != -1)
            while(itForward < _forwardFolderPath.size() - 1)
                _forwardFolderPath.pop_back();

        _currentPath = path;
        _forwardFolderPath.push_back(_currentPath);
        _backFolderPath.push_back(_currentPath);

        _listFolderContents->clear();
        _edCurrentFolder->setText(_currentPath);
        displayContent(_currentPath);
    }
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
        _listFolderContents->addItem(new QListWidgetItem(QIcon(":resource/icons/file.png"), fileName));

    this->show();
}

bool MainWindow::copyDir(QString copyPath, QString insertPath)
{
    bool success = false;
    QDir sourceDir(copyPath);

    if(!sourceDir.exists())
        return false;

    QDir destDir(insertPath);
    if(!destDir.exists())
        destDir.mkdir(insertPath);

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++) {
        QString srcName = copyPath + QDir::separator() + files[i];
        QString destName = insertPath + QDir::separator() + files[i];
        success = QFile::copy(srcName, destName);
        if(!success)
            return false;
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = copyPath + QDir::separator() + files[i];
        QString destName = insertPath + QDir::separator() + files[i];
        success = copyDir(srcName, destName);
        if(!success)
            return false;
    }

    return true;
}

void MainWindow::displayContent(QString path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 2; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isFile())
            _listFolderContents->addItem(new QListWidgetItem(QIcon(":resource/icons/file.png"), fileInfo.fileName()));
        else
            _listFolderContents->addItem(new QListWidgetItem(QIcon(":resource/icons/folder.png"), fileInfo.fileName()));
    }
}

MainWindow::~MainWindow()
{
    delete _wndCreateFile;
    delete _wndCreateFolder;
    delete _wndFileContent;
}
