/*
 * чтение/запись в файл - 2 клика (осуществляется переход в новое окно)
 * переход в дирректорию - 2 клик (на QListWidget отображать содержимое директории)
 * переход назад в директорию - будет в QListWidget выбор .. нажав на который будет осуществляться переход назад
*/

#pragma once
#include "filecontentwindow.h"
#include <createwindows.h>

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QList>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QString username, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString _username;
    QList <QString> _currentPath;
    QList <QString> _forwardFolderPath;

    QVBoxLayout* _ltMain;
    QLineEdit* _edCurrentFolder;
    QListWidget* _listFolderContents;
    QPushButton* _btCreateFile;
    QPushButton* _btCreateFolder;
    QPushButton* _btDelete;
    QPushButton* _btBackFolder;
    QPushButton* _btForwardFolder;

    FileContentWindow* _wndFileContent;
    CreateFileWindow* _wndCreateFile;
    CreateFolderWindow* _wndCreateFolder;

signals:
    void sendCurrentFile(QString);
    void openFile();
    void openFolder();
    void signCreateFile(QString);
    void signCreateFolder(QString);

    void signLoggingCreateFolder(QString);
    void signLoggingCreateFile(QString);
    void signLoggingChangeFile(QString);
    void signLoggingDeleteElement(QString);

private slots:
    void createFile();                                  // _btCreateFile
    void displayFile(QString);
    void changeFile();
    void createFolder();                                // _btCreateFolder
    void displayFolder(QString);
    void deleteElement();                               // _btDelete
    void moveFolder(QListWidgetItem*);                  // _list
    void moveBackFolder();                              // _btBackFolder
    void moveForwardFolder();                           // _btForwardFolder

    void rcvConnectCloseFile();
    void rcvConnectSaveFile(QString);

private:
    void displayContent();
    QString getFullPath();
};

