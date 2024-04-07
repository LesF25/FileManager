#include "createwindows.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QDir>
#include <QSpacerItem>

CreateFileWindow::CreateFileWindow(QWidget* parent)
    : QWidget{parent}
{
    _ltMain = new QVBoxLayout();
    this->setWindowTitle("Create File");
    this->setLayout(_ltMain);
    this->setFixedSize(370, 125);

    QLabel* lbCreateFile = new QLabel("Create File: ", this);
    lbCreateFile->setFont(QFont("Calibri", 15));
    lbCreateFile->setAlignment(Qt::AlignCenter);

    QHBoxLayout* ltName = new QHBoxLayout();
    _edFileName = new QLineEdit(this);
    _edFileName->setMaxLength(20);
    _edFileName->setPlaceholderText("File name");
    _edFileName->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^/:*?\"<>|\']{%1}").arg(_edFileName->maxLength()))));
    _edFileName->setFixedHeight(26);
    _edFileName->setFont(QFont(_edFileName->font().defaultFamily(), 10));

    _cboxFileExtension = new QComboBox(this);
    _cboxFileExtension->setFixedWidth(50);
    _cboxFileExtension->addItem(".txt");
    _cboxFileExtension->setFixedHeight(26);
    _cboxFileExtension->setFont(QFont(_cboxFileExtension->font().defaultFamily(), 9));

    ltName->addWidget(_edFileName);
    ltName->addWidget(_cboxFileExtension);

    QHBoxLayout* ltNavigation = new QHBoxLayout();
    _btCancel = new QPushButton("Cancel", this);
    _btCancel->setFixedHeight(26);
    _btCancel->setFont(QFont(_btCancel->font().defaultFamily(), 9));

    _btCreateFile = new QPushButton("Create File", this);
    _btCreateFile->setFixedHeight(26);
    _btCreateFile->setFont(QFont(_btCreateFile->font().defaultFamily(), 9));

    ltNavigation->addWidget(_btCreateFile);
    ltNavigation->addWidget(_btCancel);

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    _ltMain->addWidget(lbCreateFile);
    _ltMain->addLayout(ltName);
    _ltMain->addItem(spacer);
    _ltMain->addLayout(ltNavigation);

    connect(_btCancel, &QPushButton::clicked, this, &CreateFileWindow::cancel);
    connect(_btCreateFile, &QPushButton::clicked, this, &CreateFileWindow::createFile);
}

void CreateFileWindow::closeEvent(QCloseEvent *event)
{
    emit signCloseWindow();
    event->accept();
}

void CreateFileWindow::createFile()
{
    if (_edFileName->displayText().isEmpty())
    {
        QMessageBox::warning(0, "Error", "Enter the file name.");
        return;
    }

    QString fileName = _edFileName->text();
    QString path = _path + fileName + _cboxFileExtension->currentText();
    QFile file(path);

    if (file.exists(path))
    {
        QMessageBox::warning(0, "Error", "File already exists");
        return;
    }

    _edFileName->clear();
    this->close();
    emit signCreateFile(path);
}

void CreateFileWindow::rcvConnect(QString path)
{
    _path = path;
}

void CreateFileWindow::cancel()
{
    _edFileName->clear();

    this->close();
    emit signCloseWindow();
}

CreateFileWindow::~CreateFileWindow()
{
}

//===============================================================================================

CreateFolderWindow::CreateFolderWindow(QWidget* parent)
    : QWidget{parent}
{
    _ltMain = new QVBoxLayout();
    this->setWindowTitle("Create Folder");
    this->setLayout(_ltMain);
    this->setFixedSize(370, 125);

    QLabel* lbCreateFodler = new QLabel("Create Folder: ", this);
    lbCreateFodler->setFont(QFont("Calibri", 15));
    lbCreateFodler->setAlignment(Qt::AlignCenter);

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

    _btCreateFolder = new QPushButton("Create Folder", this);
    _btCreateFolder->setFixedHeight(26);
    _btCreateFolder->setFont(QFont(_btCreateFolder->font().defaultFamily(), 9));

    ltNavigation->addWidget(_btCreateFolder);
    ltNavigation->addWidget(_btCancel);

    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    _ltMain->addWidget(lbCreateFodler);
    _ltMain->addWidget(_edFolderName);
    _ltMain->addItem(spacer);
    _ltMain->addLayout(ltNavigation);

    connect(_btCancel, &QPushButton::clicked, this, &CreateFolderWindow::cancel);
    connect(_btCreateFolder, &QPushButton::clicked, this, &CreateFolderWindow::createFolder);
}

void CreateFolderWindow::closeEvent(QCloseEvent *event)
{
    emit signCloseWindow();
    event->accept();
}

void CreateFolderWindow::createFolder()
{
    if (_edFolderName->displayText().isEmpty())
    {
        QMessageBox::warning(0, "Error", "Enter the folder name.");
        return;
    }

    QString folderName = _edFolderName->text();
    QString path = _path + folderName + "/";
    QDir dir(path);

    if (dir.exists())
    {
        QMessageBox::warning(0, "Error", "Folder already exists");
        return;
    }

    dir.mkdir(path);
    _edFolderName->clear();
    this->close();
    emit signCreateFolder(folderName);
}

void CreateFolderWindow::rcvConnect(QString path)
{
    _path = path;
}

void CreateFolderWindow::cancel()
{
    _edFolderName->clear();

    this->close();
    emit signCloseWindow();
}

CreateFolderWindow::~CreateFolderWindow()
{
}
