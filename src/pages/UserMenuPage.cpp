#include "../../include/pages/UserMenuPage.h"

#include <QVBoxLayout>

UserMenuPage::UserMenuPage(QWidget *parent) : QWidget(parent) {
    available_ = new QPushButton("Available catalog", this);
    rented_ = new QPushButton("Rented books", this);
    logout_ = new QPushButton("Log out", this);

    available_->setMinimumHeight(40);
    rented_->setMinimumHeight(40);

    auto *root = new QVBoxLayout(this);
    root->addWidget(available_);
    root->addWidget(rented_);
    root->addStretch();
    root->addWidget(logout_);

    connect(available_, &QPushButton::clicked, this, &UserMenuPage::go_available);
    connect(rented_, &QPushButton::clicked, this, &UserMenuPage::go_rented);
    connect(logout_, &QPushButton::clicked, this, &UserMenuPage::logout);
}

void UserMenuPage::set_user(UserBase *user) {
    user_ = user;
}
