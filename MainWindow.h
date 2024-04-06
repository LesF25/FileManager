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
#include <memory>

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
    QPushButton* _btCreateFile;
    QPushButton* _btCreateFolder;
    QPushButton* _btDelete;
    QPushButton* _btExitAccount;
    QPushButton* _btBackFolder;
    QPushButton* _btForwardFolder;
    IMQuickAccess* _listQuickAccess;

    LoginWindow* _wndLogin;
    FileContentWindow* _wndFileContent;
    CreateFileWindow* _wndCreateFile;
    CreateFolderWindow* _wndCreateFolder;

signals:
    void sendCurrentFile(QString);
    void openFile();
    void openFolder();

    void signCreateFile(QString);
    void signCreateFolder(QString);
    void signQuickMoveFolder();

private slots:
    void createFile();                                  // _btCreateFile
    void displayFile(QString);
    void changeFile();
    void createFolder();                                // _btCreateFolder
    void displayFolder(QString);
    void deleteElement();                               // _btDelete
    void exitAccount();                                 // _btExitAccount
    void moveFolder(QListWidgetItem*);                  // _list
    void moveBackFolder();                              // _btBackFolder
    void moveForwardFolder();                           // _btForwardFolder
    void quickMoveFodler();

    void rcvConnectLogIn(QString);
    void rcvConnectCloseFile();
    void rcvConnectSaveFile(QString);
    void rcvConnectQuickAccess(QString);

private:
    void initStyleLabel(QLabel* label, int width, Qt::AlignmentFlag flag);
    void displayContent(QString);
};

