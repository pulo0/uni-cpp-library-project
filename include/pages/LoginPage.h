#ifndef LIBRARY_PROJECT_LOGINPAGE_H
#define LIBRARY_PROJECT_LOGINPAGE_H
#include "../auth/Authentication.h"
#include "../user/UserBase.h"
#include <QLineEdit>
#include <QPushButton>

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);

signals:
    void login_success(UserBase* user);

private slots:
    void on_login_clicked();
    void on_register_clicked();
    void on_forgot_passwd_clicked();

private:
    void validate_input() const;

    Authentication auth_;
    QLineEdit *login_ = nullptr;
    QLineEdit *passwd_ = nullptr;
    QPushButton *confirm_ = nullptr;
    QPushButton *register_ = nullptr;
    QPushButton *forgot_passwd_ = nullptr;

};

#endif