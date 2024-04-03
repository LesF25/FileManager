#include <QWidget>
#include <QString>
#include <QMap>
#include <QtSql>

class IdentityMap
{
public:
    IdentityMap();
    ~IdentityMap() = default;

    bool signIn(QString, QString, QString);
    bool signUp(QString, QString, QString);

private:
    QMap<QString, QString> _users;
    QSqlDatabase _dbUsers;
};
