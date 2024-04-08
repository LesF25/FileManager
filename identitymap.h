#include <QWidget>
#include <QString>
#include <QtSql>
#include <QListWidget>
#include <QMultiMap>

class IMLoginSystem
{
public:
    IMLoginSystem();
    ~IMLoginSystem() = default;

    bool signIn(QString, QString, QString);
    bool signUp(QString, QString, QString);

private:
    QMap<QString, QString> _users;
    QSqlDatabase _dbUsers;
};

class IMQuickAccess : public QListWidget
{
    Q_OBJECT
public:
    IMQuickAccess() = default;
    ~IMQuickAccess() = default;

    void addQuickAccess(QString);
    void removeQuickAccess(int);
private:
    QList<QString> _quickAccessToFolders;

    bool search(QString);

signals:
    void signSendPath(QString);

public slots:
    void quickMoveFolder();

};
