#pragma once

#include <QObject>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QCloseEvent>

class CreateFileWindow : public QWidget
{
    Q_OBJECT

public:
    CreateFileWindow(QWidget *parent = nullptr);
    ~CreateFileWindow();

private:
    QString _path;
    QVBoxLayout* _ltMain;
    QLineEdit* _edFileName;
    QComboBox* _cboxFileExtension;
    QPushButton* _btCancel;
    QPushButton* _btCreateFile;

private:
    void closeEvent(QCloseEvent*);

signals:
    void signCreateFile(QString);
    void signCloseWindow();

private slots:
    void createFile();
    void cancel();

public slots:
    void rcvConnect(QString);
};


class CreateFolderWindow : public QWidget
{
    Q_OBJECT

public:
    CreateFolderWindow(QWidget *parent = nullptr);
    ~CreateFolderWindow();

private:
    QString _path;
    QVBoxLayout* _ltMain;
    QLineEdit* _edFolderName;
    QPushButton* _btCancel;
    QPushButton* _btCreateFolder;

private:
    void closeEvent(QCloseEvent*);

signals:
    void signCreateFolder(QString);
    void signCloseWindow();

private slots:
    void createFolder();
    void cancel();

public slots:
    void rcvConnect(QString);
};
