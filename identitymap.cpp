#include "identitymap.h"
#include <QMessageBox>

IdentityMap::IdentityMap()
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

bool IdentityMap::signIn(QString username, QString password, QString email)
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

bool IdentityMap::signUp(QString username, QString password, QString email)
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
