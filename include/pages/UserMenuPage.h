#ifndef LIBRARY_PROJECT_USERMENUPAGE_H
#define LIBRARY_PROJECT_USERMENUPAGE_H
#include <QPushButton>
#include <QWidget>

#include "../user/UserBase.h"

class UserMenuPage : public QWidget {
    Q_OBJECT;

public:
    explicit UserMenuPage(QWidget *parent = nullptr);

    void set_user(UserBase *user);

signals:
    void go_available();

    void go_rented();

    void logout();

private:
    UserBase *user_ = nullptr;
    QPushButton *available_ = nullptr;
    QPushButton *rented_ = nullptr;
    QPushButton *logout_ = nullptr;
};

#endif
