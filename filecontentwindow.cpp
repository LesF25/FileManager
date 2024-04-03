#include "filecontentwindow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QTextStream>
#include <QFileInfo>

FileContentWindow::FileContentWindow(QWidget *parent)
    : QWidget{parent}
{
    _ltMain = new QVBoxLayout();
    this->setLayout(_ltMain);
    this->setFixedSize(350, 350);

    this->setWindowTitle("Editing window");
    //========================================================
    QHBoxLayout* ltBody = new QHBoxLayout();
    _plFileContents = new QPlainTextEdit(this);
    _plFileContents->setFont(QFont(_plFileContents->font().defaultFamily(), 10));
    ltBody->addWidget(_plFileContents);

    //========================================================

    QHBoxLayout* ltFooter = new QHBoxLayout();
    _btSaveFileContents = new QPushButton("Save file", this);
    _btSaveFileContents->setFixedHeight(26);
    _btSaveFileContents->setFont(QFont(_btSaveFileContents->font().defaultFamily(), 9.5));

    _btCancel = new QPushButton("Cancel", this);
    _btCancel->setFixedHeight(26);
    _btCancel->setFont(QFont(_btCancel->font().defaultFamily(), 9.5));

    ltFooter->addWidget(_btSaveFileContents);
    ltFooter->addWidget(_btCancel);

    //========================================================
    _ltMain->addLayout(ltBody);
    _ltMain->addLayout(ltFooter);

    connect(_btSaveFileContents, &QPushButton::clicked, this, &FileContentWindow::saveFileContents);
    connect(_btCancel, &QPushButton::clicked, this, &FileContentWindow::closeWnd);
}

void FileContentWindow::closeWnd()
{
    this->close();
    emit fileClose();
}

void FileContentWindow::rcvCurrentFile(QString path)
{
    _file.setFileName(path);
    QFileInfo fileInfo(_file);

    if(_file.exists() && fileInfo.isFile())
    {
        bool openFile = _file.open(QIODevice::ReadWrite);
        if (openFile)
        {
            QString text;
            QTextStream textStream(&_file);
            while(!textStream.atEnd())
            {
                text += textStream.readLine() + "\n";
            }
            _plFileContents->setPlainText(text);

            _file.close();
        }
    }
}

void FileContentWindow::saveFileContents()
{
    bool openFile = _file.open(QIODevice::ReadWrite);
    if (openFile)
    {
        QTextStream textStream(&_file);
        textStream << _plFileContents->toPlainText();

        _file.close();
    }

    QFileInfo fileInfo(_file);
    this->close();
    emit fileSaved(fileInfo.fileName());
}

FileContentWindow::~FileContentWindow()
{
}
