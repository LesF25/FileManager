#include "loginwindow.h"

#include <QHBoxLayout>
#include <QVector>
#include <QFileInfo>
#include <QDir>
#include <QSpacerItem>


LoginWindow::LoginWindow(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("Registration window");
    QString path = "C:/Users/getd8/Desktop/FileManager/users/";

    if (!_dir.exists(path))
        _dir.mkdir(path);

    _identityMap = new IdentityMap();

    _ltMain = new QVBoxLayout();
    this->setLayout(_ltMain);
    this->setFixedSize(340, 215);

    //==========================================
    QVBoxLayout* ltBody = new QVBoxLayout();

    QLabel* lbLogin = new QLabel("Login", this);
    lbLogin->setFont(QFont("Calibri", 19.5));
    lbLogin->setAlignment(Qt::AlignCenter);

    _edEmail = new QLineEdit(this);
    _edEmail->setPlaceholderText("Email");
    _edEmail->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^а-яА-яёЁ/:*?\"<>|\']{%1}").arg(_edEmail->maxLength()))));
    _edEmail->setFixedHeight(28);
    _edEmail->setFont(QFont(_edEmail->font().defaultFamily(), 10));

    _edLogin = new QLineEdit(this);
    _edLogin->setMaxLength(15);
    _edLogin->setPlaceholderText("User name");
    _edLogin->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^а-яА-яёЁ/:*?\"<>|\']{%1}").arg(_edLogin->maxLength()))));
    _edLogin->setFixedHeight(28);
    _edLogin->setFont(QFont(_edLogin->font().defaultFamily(), 10));

    _edPassword = new QLineEdit(this);
    _edPassword->setMaxLength(10);
    _edPassword->setPlaceholderText("password (max length 10)");
    _edPassword->setValidator(new QRegularExpressionValidator(QRegularExpression(QString("[^а-яА-яёЁ/:*?\"<>|\']{%1}").arg(_edPassword->maxLength()))));
    _edPassword->setFixedHeight(28);
    _edPassword->setFont(QFont(_edPassword->font().defaultFamily(), 10));

    ltBody->addWidget(lbLogin);
    ltBody->addWidget(_edLogin);
    ltBody->addWidget(_edPassword);
    ltBody->addWidget(_edEmail);

    //==========================================
    QHBoxLayout* ltControlTools = new QHBoxLayout();

    _btSignIn = new QPushButton("Sign in", this);
    _btSignIn->setFixedHeight(27.5);
    _btSignIn->setFont(QFont(_btSignIn->font().defaultFamily(), 9.5));

    _btSignUp = new QPushButton("SignUp", this);
    _btSignUp->setFixedHeight(27.5);
    _btSignUp->setFont(QFont(_btSignUp->font().defaultFamily(), 9.5));

    ltControlTools->addWidget(_btSignIn);
    ltControlTools->addWidget(_btSignUp);

    //==========================================
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    _ltMain->addLayout(ltBody);
    _ltMain->addItem(spacer);
    _ltMain->addLayout(ltControlTools);

    //==========================================
    connect(_btSignUp, &QPushButton::clicked, this, &LoginWindow::signUp);
    connect(_btSignIn, &QPushButton::clicked, this, &LoginWindow::signIn);
}

void LoginWindow::signUp()
{
    if (_edLogin->text().length() < 3) {
        QMessageBox::warning(0, "Error", "The login must consist of at least 3 characters.");
        return;
    }

    if (_edPassword->text().length() < 5) {
        QMessageBox::warning(0, "Error", "The password must consist of at least 5 characters.");
        return;
    }

    if (_edEmail->text().length() < 6) {
        QMessageBox::warning(0, "Error", "The email must consist of at least 6 characters.");
        return;
    }

    if (!_identityMap->signUp(_edLogin->text(), _edPassword->text(), _edEmail->text()))
    {
        QMessageBox::warning(0, "Error", "Failed to create a new user.");
        return;
    }

    QString path = "C:/Users/getd8/Desktop/FileManager/users/" + _edLogin->text() + "/";
    if (!_dir.exists(path))
        _dir.mkdir(path);

    this->close();
    emit logIn(_edLogin->text());
}

void LoginWindow::signIn()
{
    if (_edLogin->displayText().isEmpty() || _edPassword->displayText().isEmpty() || _edEmail->displayText().isEmpty())
    {
        QMessageBox::warning(0, "Error", "Fill in all the fields for registration.");
        return;
    }

    if (!_identityMap->signIn(_edLogin->text(), _edPassword->text(), _edEmail->text()))
    {
        QMessageBox::warning(0, "Error", "Couldn't log in to your account.");
        return;
    }

    this->close();
    emit logIn(_edLogin->text());
}

LoginWindow::~LoginWindow()
{
}
