#pragma once
#include "filecontentwindow.h"
#include "createwindows.h"

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QList>
#include <QToolBar>
#include <QAction>

#include <QDir>
#include <QFile>
#include <QFileInfo>

class IMQuickAccess;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QString, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString _username;
    QString _currentPath;
    QList <QString> _forwardFolderPath;
    QList <QString> _backFolderPath;

    QVBoxLayout* _ltMain;
    QToolBar* _toolBar;
    QLineEdit* _edCurrentFolder;
    QListWidget* _listFolderContents;
    IMQuickAccess* _listQuickAccess;

    QAction* _actBackFolder;
    QAction* _actForwardFolder;
    QAction* _actCreateFolder;
    QAction* _actCreateFile;
    QAction* _actDelete;
    QAction* _actExitAccount;
    QAction* _actDownload;

    FileContentWindow* _wndFileContent;
    CreateFileWindow* _wndCreateFile;
    CreateFolderWindow* _wndCreateFolder;

signals:
    void signSendCurrentFile(QString);
    void signOpenFile();

    void signCreateFile(QString);
    void signCreateFolder(QString);
    void signQuickMoveFolder(int);
    void signExitAccount();

private slots:
    void createFile();
    void createFolder();
    void deleteElement();
    void exitAccount();
    void moveBackFolder();
    void moveForwardFolder();

    void displayFolder(QString);
    void displayContentFile(QString);
    void changeFile();

    void moveFolder(QListWidgetItem*);
    void clearChoiceContentList();
    void showContextMenuContentList(const QPoint& pos);

    void quickMoveFolder();
    void clearChoiceQuickList();
    void showContextMenuQuickList(const QPoint& pos);
    void fixFolder();

    void rcvConnectSaveFile(QString);
    void rcvConnectQuickAccess(QString);
    void setAccessMainWindow() {this->show();}
    void detachFolder();

private:
    void displayContent(QString);
};




class IMQuickAccess : public QListWidget
{
    Q_OBJECT

private:
    QList<QString> _quickAccessToFolders;

public:
    IMQuickAccess() = default;

    ~IMQuickAccess() = default;

    void addQuickAccess(QString pathAbs)
    {
        if (search(pathAbs))
            return;

        QFile file(pathAbs);
        QFileInfo fileInfo(file);

        if(file.exists() && fileInfo.isFile())
        {
            _quickAccessToFolders.push_back(pathAbs);
            this->addItem(new QListWidgetItem(QIcon(":resource/icons/file.png"), fileInfo.fileName()));

            return;
        }

        QDir dir(pathAbs);
        QFileInfo dirInfo(dir.absolutePath());

        if (dir.exists() && dirInfo.isDir())
        {
            _quickAccessToFolders.push_back(pathAbs);
            this->addItem(new QListWidgetItem(QIcon(":resource/icons/folder.png"), dirInfo.fileName()));

            return;
        }
    }

    void removeQuickAccess(int index)
    {
        _quickAccessToFolders.removeAt(index);
        this->takeItem(index);
    }

private:
    bool search(QString path)
    {
        int it = _quickAccessToFolders.indexOf(path);
        if (it == -1)
            return false;

        return true;
    }

signals:
    void signSendPath(QString);

public slots:
    void quickMoveFolder(int index)
    {
        emit signSendPath(_quickAccessToFolders.at(index));
    }
};
