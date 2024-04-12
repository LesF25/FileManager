#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QCloseEvent>
#include <QDir>
#include <QFile>


class RenameFileWindow : public QWidget
{
    Q_OBJECT

public:
    RenameFileWindow(QWidget *parent = nullptr);
    ~RenameFileWindow() = default;

private:
    QString _path;
    QString _extention;
    QVBoxLayout* _ltMain;
    QLineEdit* _edFileName;
    QPushButton* _btCancel;
    QPushButton* _btRenameFile;

private:
    void closeEvent(QCloseEvent*);

signals:
    void signRenameFile(QString);
    void signCloseWindow();

private slots:
    void renameFile();
    void cancel();

public slots:
    void rcvConnect(QString);
};




class RenameFolderWindow : public QWidget
{
    Q_OBJECT

public:
    RenameFolderWindow(QWidget *parent = nullptr);
    ~RenameFolderWindow() = default;

private:
    QString _path;
    QVBoxLayout* _ltMain;
    QLineEdit* _edFolderName;
    QPushButton* _btCancel;
    QPushButton* _btRenameFolder;

private:
    void closeEvent(QCloseEvent*);

signals:
    void signRenameFolder(QString);
    void signCloseWindow();

private slots:
    void renameFolder();
    void cancel();

public slots:
    void rcvConnect(QString);
};
