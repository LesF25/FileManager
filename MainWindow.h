#pragma once
#include "loginwindow.h"
#include "filecontentwindow.h"
#include "createwindows.h"

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QList>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString _username;
    QString _currentPath;
    QList <QString> _forwardFolderPath;
    QList <QString> _backFolderPath;

    QVBoxLayout* _ltMain;
    QLineEdit* _edCurrentFolder;
    QListWidget* _listFolderContents;
    IMQuickAccess* _listQuickAccess;
    QPushButton* _btCreateFile;
    QPushButton* _btCreateFolder;
    QPushButton* _btDelete;
    QPushButton* _btExitAccount;
    QPushButton* _btBackFolder;
    QPushButton* _btForwardFolder;

    LoginWindow* _wndLogin;
    FileContentWindow* _wndFileContent;
    CreateFileWindow* _wndCreateFile;
    CreateFolderWindow* _wndCreateFolder;

signals:
    void signSendCurrentFile(QString);
    void signOpenFile();

    void signCreateFile(QString);
    void signCreateFolder(QString);
    void signQuickMoveFolder(int);

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
    void clearChoiceContentList() {_listFolderContents->clearSelection();}
    void showContextMenuContentList(const QPoint& pos);

    void quickMoveFolder() { emit signQuickMoveFolder(_listQuickAccess->currentRow()); }
    void clearChoiceQuickList() {_listQuickAccess->clearSelection();}
    void showContextMenuQuickList(const QPoint& pos);
    void fixFolder();

    void rcvConnectLogIn(QString);
    void rcvConnectSaveFile(QString);
    void rcvConnectQuickAccess(QString);
    void setAccessMainWindow() {this->show();}
    void detachFolder();

private:
    void initStyleLabel(QLabel* label, int width, Qt::AlignmentFlag flag);
    void displayContent(QString);
};

