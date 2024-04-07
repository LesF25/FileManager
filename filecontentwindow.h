#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QCloseEvent>

class FileContentWindow : public QWidget
{
    Q_OBJECT

public:
    FileContentWindow(QWidget *parent = nullptr);
    ~FileContentWindow();

private:
    QVBoxLayout* _ltMain;
    QPlainTextEdit* _plFileContents;                    // будет осуществлять вывод содержимого файла, а также позволять его изменять
    QPushButton* _btSaveFileContents;
    QPushButton* _btCancel;
    QFile _file;

private:
    void closeEvent(QCloseEvent*);

signals:
    void fileSaved(QString);
    void signCloseWindow();

private slots:
    void saveFileContents();
    void closeWnd();

public slots:
    void rcvCurrentFile(QString);
};
