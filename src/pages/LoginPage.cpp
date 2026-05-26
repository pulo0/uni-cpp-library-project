#include "../../include/pages/LoginPage.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QInputDialog>
#include <iostream>

LoginPage::LoginPage(QWidget *parent) : QWidget(parent) {
    login_ = new QLineEdit(this);
    login_->setPlaceholderText("login");

    passwd_ = new QLineEdit(this);
    passwd_->setPlaceholderText("password");
    passwd_->setEchoMode(QLineEdit::Password);

    confirm_ = new QPushButton("Confirm", this);
    register_ = new QPushButton("Register", this);
    forgot_passwd_ = new QPushButton("Forgot Password", this);

    auto *form = new QFormLayout;
    form->addRow(login_);
    form->addRow(passwd_);

    auto *buttons = new QHBoxLayout;
    buttons->addWidget(confirm_);
    buttons->addWidget(register_);
    buttons->addWidget(forgot_passwd_);

    auto *root = new QVBoxLayout(this);
    root->addLayout(form);
    root->addLayout(buttons);
    root->addStretch();

    connect(confirm_, &QPushButton::clicked, this, &LoginPage::on_login_clicked);
    connect(register_, &QPushButton::clicked, this, &LoginPage::on_register_clicked);
    connect(forgot_passwd_, &QPushButton::clicked, this, &LoginPage::on_forgot_passwd_clicked);

    connect(login_, &QLineEdit::textChanged, this, &LoginPage::validate_input);
    connect(passwd_, &QLineEdit::textChanged, this, &LoginPage::validate_input);

    validate_input();
}

void LoginPage::validate_input() const {
    const bool ok = !login_->text().trimmed().isEmpty() && !passwd_->text().trimmed().isEmpty();
    confirm_->setEnabled(ok);
}

void LoginPage::on_login_clicked() {
    try {
        auto user = auth_.login(login_->text().toStdString(), passwd_->text().toStdString());
        emit login_success(user.release());
    } catch (const std::exception &exc) {
        QMessageBox::warning(this, "Login failure", exc.what());
    }
}

void LoginPage::on_register_clicked() {
    const QString login = login_->text().trimmed();

    bool ok;
    const QString passwd = QInputDialog::getText(this, "Register", "Password:", QLineEdit::Password, "", &ok);

    if (!ok) return;

    bool ok_conf;
    const QString passwd_conf = QInputDialog::getText(this, "Register", "Confirm password:", QLineEdit::Password, "",
                                                      &ok_conf);

    if (!ok_conf) return;

    if (passwd != passwd_conf) {
        QMessageBox::warning(this, "Registration failure", "Passwords do not match each other.");
        return;
    }

    if (login.isEmpty()) {
        QMessageBox::warning(this, "Registration failure", "Please provide login information (first field)");
        return;
    }

    try {
        auto user = auth_.register_user(login.toStdString(), passwd.toStdString());
        emit login_success(user.release());
    } catch (const std::exception &exc) {
        QMessageBox::warning(this, "Registration failure", exc.what());
    }
}

void LoginPage::on_forgot_passwd_clicked() {
    const QString login = login_->text().trimmed();
    if (login.isEmpty()) {
        QMessageBox::information(this, "Forgot password", "Please provide login first");
        return;
    }

    bool ok;
    const QString new_passwd = QInputDialog::getText(this, "Forgot password", "New password:", QLineEdit::Password, "",
                                                     &ok);
    if (!ok) return;

    bool ok_conf;
    const QString new_passwd_conf = QInputDialog::getText(this, "Forgot password", "Confirm Password:",
                                                          QLineEdit::Password, "", &ok_conf);
    if (!ok_conf) return;

    try {
        auth_.forgot_password(login.toStdString(), new_passwd.toStdString(), new_passwd_conf.toStdString());
        QMessageBox::information(this, "Password reset", "Successfully changed password!");
    } catch (const std::exception &exc) {
        QMessageBox::warning(this, "Password change failure", exc.what());
    }
}
