#include "identitymap.h"
#include <QMessageBox>
#include <QFile>

IMLoginSystem::IMLoginSystem()
{
    _dbUsers = QSqlDatabase::addDatabase("QSQLITE");
    _dbUsers.setDatabaseName("C:/Users/getd8/Desktop/FileManager/users.db");
    bool checkOpen = _dbUsers.open();

    if (!checkOpen)
    {
        QMessageBox::warning(0, "Error", _dbUsers.lastError().text());
        return;
    }

    _dbUsers.transaction();

    QSqlQuery* query = new QSqlQuery(_dbUsers);
    query->exec("CREATE TABLE IF NOT EXISTS Users("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "login TEXT NOT NULL UNIQUE,"
                "password TEXT NOT NULL,"
                "email TEXT NOT NULL UNIQUE"
                ");"
                );

    _dbUsers.commit();
    _dbUsers.close();
}

bool IMLoginSystem::signIn(QString username, QString password, QString email)
{
    if (!_users.isEmpty())
    {
        QMapIterator<QString, QString> it(_users);
        while(it.hasNext())
        {
            it.next();
            if (it.key() == username && it.value() == password)
                return true;
        }
    }

    bool checkOpen = _dbUsers.open();
    if (!checkOpen)
    {
        QMessageBox::warning(0, "Error", _dbUsers.lastError().text());
        return false;
    }
    _dbUsers.transaction();

    QSqlQuery* query = new QSqlQuery(_dbUsers);
    query->prepare(QString("SELECT * FROM Users WHERE login = '%1' AND password = '%2' AND email = '%3'").arg(username, password, email));
    query->exec();

    if (!query->next())
        return false;

    _dbUsers.commit();
    _dbUsers.close();

    _users.insert(username, password);
    return true;
}

bool IMLoginSystem::signUp(QString username, QString password, QString email)
{
    bool checkOpen = _dbUsers.open();
    if (!checkOpen)
    {
        QMessageBox::warning(0, "Error", _dbUsers.lastError().text());
        return false;
    }
    _dbUsers.transaction();

    QSqlQuery* query = new QSqlQuery(_dbUsers);
    query->prepare("INSERT INTO Users(login, password, email) VALUES(:login, :password, :email)");
    query->bindValue(":login", username);
    query->bindValue(":password", password);
    query->bindValue(":email", email);
    bool resultCreateUser = query->exec();

    if (!resultCreateUser)
        return false;

    _dbUsers.commit();
    _dbUsers.close();

    _users.insert(username, password);

    return true;
}

// ================================================================================
void IMQuickAccess::quickMoveFolder()
{
    emit signSendPath(_quickAccessToFolders[this->currentRow()]);
}

void IMQuickAccess::addQuickAccess(QString pathAbs)
{
    if (search(pathAbs))
        return;

    QFile file(pathAbs);
    QFileInfo fileInfo(file);

    if(file.exists() && fileInfo.isFile())
    {
        _quickAccessToFolders.push_back(pathAbs);
        this->addItem(new QListWidgetItem(QIcon(":resource/icons/file.png"), fileInfo.fileName()));
    }
    else if (file.exists() && fileInfo.isDir())
    {
        QDir dir(pathAbs);
        fileInfo.setFile(dir.absolutePath());
        _quickAccessToFolders.push_back(pathAbs);
        this->addItem(new QListWidgetItem(QIcon(":resource/icons/folder.png"), fileInfo.fileName()));
    }
}

void IMQuickAccess::removeQuickAccess(int index)
{
    _quickAccessToFolders.removeAt(index);
    this->takeItem(index);
}

bool IMQuickAccess::search(QString path)
{
    int it = _quickAccessToFolders.indexOf(path);
    if (it == -1)
        return false;

    return true;
}
