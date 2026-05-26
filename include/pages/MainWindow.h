#ifndef LIBRARY_PROJECT_MAINWINDOW_H
#define LIBRARY_PROJECT_MAINWINDOW_H
#include "../router/AppRouter.h"
#include "../user/UserBase.h"
#include <QMainWindow>

#include "AdminPage.h"
#include "LoginPage.h"
#include "UserAvailablePage.h"
#include "UserMenuPage.h"
#include "UserRentedPage.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void on_login_success(UserBase *user);

    void on_logout();

private:
    void build_pages();

    void proceed_to_login() const;

    void proceed_after_login() const;

    // User data persistent through pages
    std::unique_ptr<UserBase> current_user_;

    // Page switching utilities
    QStackedWidget *stack_ = nullptr;
    AppRouter *router_;

    LoginPage *login_page_;
    UserMenuPage *user_menu_page_;
    UserAvailablePage *user_available_page_;
    UserRentedPage *user_rented_page_;
    AdminPage *admin_page_;
};

#endif
