#include "renamewindows.h"

#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QSpacerItem>

RenameFileWindow::RenameFileWindow(QWidget* parent)
    : QWidget{parent}
{
    _ltMain = new QVBoxLayout();
    this->setWindowTitle("Rename File");
    this->setLayout(_ltMain);
    this->setFixedSize(370, 125);

    QLabel* lbRenameFile = new QLabel("Rename File: ", this);
    lbRenameFile->setFont(QFont("Calibri", 15));
    lbRenameFile->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ltName = new QHBoxLayout();
    _edFileName = new QLineEdit(this);
    _edFileName->setMaxLength(20);
    _edFileName->setPlaceholderText("File name");
    _edFileName->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^/:*?\"<>|\']{%1}").arg(_edFileName->maxLength()))));
    _edFileName->setFixedHeight(26);
    _edFileName->setFont(QFont(_edFileName->font().defaultFamily(), 10));

    ltName->addWidget(_edFileName);

    QHBoxLayout* ltNavigation = new QHBoxLayout();
    _btCancel = new QPushButton("Cancel", this);
    _btCancel->setFixedHeight(26);
    _btCancel->setFont(QFont(_btCancel->font().defaultFamily(), 9));

    _btRenameFile = new QPushButton("Rename File", this);
    _btRenameFile->setFixedHeight(26);
    _btRenameFile->setFont(QFont(_btRenameFile->font().defaultFamily(), 9));

    ltNavigation->addWidget(_btRenameFile);
    ltNavigation->addWidget(_btCancel);

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    _ltMain->addWidget(lbRenameFile);
    _ltMain->addLayout(ltName);
    _ltMain->addItem(spacer);
    _ltMain->addLayout(ltNavigation);

    connect(_btCancel, &QPushButton::clicked, this, &RenameFileWindow::cancel);
    connect(_btRenameFile, &QPushButton::clicked, this, &RenameFileWindow::renameFile);
}

void RenameFileWindow::closeEvent(QCloseEvent *event)
{
    emit signCloseWindow();
    event->accept();
}

void RenameFileWindow::renameFile()
{
    if (_edFileName->displayText().isEmpty())
    {
        QMessageBox::warning(0, "Error", "Enter the file name.");
        return;
    }

    QString path = _path + _edFileName->text() + _extention;
    QFile file(path);

    if (file.exists(path))
    {
        QMessageBox::warning(0, "Error", "File already exists");
        return;
    }

    _edFileName->clear();
    this->close();
    emit signRenameFile(path);
}

void RenameFileWindow::rcvConnect(QString path)
{
    _path = path;

    QFile file(path);
    QFileInfo fileInfo(file);
    _extention = "." + fileInfo.suffix();

    _path.remove(fileInfo.fileName());
    _edFileName->setText(fileInfo.baseName());
}

void RenameFileWindow::cancel()
{
    _edFileName->clear();
    _path.clear();
    _extention.clear();

    this->close();
    emit signCloseWindow();
}

//===============================================================================================

RenameFolderWindow::RenameFolderWindow(QWidget* parent)
    : QWidget{parent}
{
    _ltMain = new QVBoxLayout();
    this->setWindowTitle("Rename Folder");
    this->setLayout(_ltMain);
    this->setFixedSize(370, 125);

    QLabel* lbRenameFodler = new QLabel("Rename Folder: ", this);
    lbRenameFodler->setFont(QFont("Calibri", 15));
    lbRenameFodler->setAlignment(Qt::AlignCenter);

    _edFolderName = new QLineEdit(this);
    _edFolderName->setMaxLength(20);
    _edFolderName->setPlaceholderText("Folder name");
    _edFolderName->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^/:*?\"<>|\']{%1}").arg(_edFolderName->maxLength()))));
    _edFolderName->setFixedHeight(26);
    _edFolderName->setFont(QFont(_edFolderName->font().defaultFamily(), 10));

    QHBoxLayout* ltNavigation = new QHBoxLayout();
    _btCancel = new QPushButton("Cancel", this);
    _btCancel->setFixedHeight(26);
    _btCancel->setFont(QFont(_btCancel->font().defaultFamily(), 9));

    _btRenameFolder = new QPushButton("Rename Folder", this);
    _btRenameFolder->setFixedHeight(26);
    _btRenameFolder->setFont(QFont(_btRenameFolder->font().defaultFamily(), 9));

    ltNavigation->addWidget(_btRenameFolder);
    ltNavigation->addWidget(_btCancel);

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    _ltMain->addWidget(lbRenameFodler);
    _ltMain->addWidget(_edFolderName);
    _ltMain->addItem(spacer);
    _ltMain->addLayout(ltNavigation);

    connect(_btCancel, &QPushButton::clicked, this, &RenameFolderWindow::cancel);
    connect(_btRenameFolder, &QPushButton::clicked, this, &RenameFolderWindow::renameFolder);
}

void RenameFolderWindow::closeEvent(QCloseEvent *event)
{
    emit signCloseWindow();
    event->accept();
}

void RenameFolderWindow::renameFolder()
{
    if (_edFolderName->displayText().isEmpty())
    {
        QMessageBox::warning(0, "Error", "Enter the folder name.");
        return;
    }

    QString path = _path + _edFolderName->text() + "/";
    QDir dir(path);

    if (dir.exists())
    {
        QMessageBox::warning(0, "Error", "Folder already exists");
        return;
    }

    _edFolderName->clear();
    this->close();
    emit signRenameFolder(path);
}

void RenameFolderWindow::rcvConnect(QString path)
{
    _path = path;

    QDir dir(path);
    QFileInfo fileInfo(dir.absolutePath());

    _path.remove(QString(fileInfo.fileName() + "/"));
    _edFolderName->setText(fileInfo.fileName());
}

void RenameFolderWindow::cancel()
{
    _edFolderName->clear();
    _path.clear();

    this->close();
    emit signCloseWindow();
}
