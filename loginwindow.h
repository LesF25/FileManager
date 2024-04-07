#pragma once
#include "identitymap.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>

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

    QDir _dir;
    IMLoginSystem* _identityMap;
private:
    void initStyleLabel(QLabel*, int, Qt::AlignmentFlag);
    void closeEvent(QCloseEvent*);

signals:
    void logIn(QString);
    void signCloseWindow();

private slots:
    void signIn();
    void signUp();
};
