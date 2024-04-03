#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QtSql>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    QVBoxLayout* _ltMain;
    QLineEdit* _edEmail;
    QLineEdit* _edLogin;
    QLineEdit* _edPassword;
    QPushButton* _btSignIn;
    QPushButton* _btSignUp;

    QSqlDatabase _dbUsers;
    QDir _dir;

private:
    void initStyleLabel(QLabel*, int, Qt::AlignmentFlag);

signals:
    void logIn(QString);

private slots:
    void signIn();
    void signUp();
};
