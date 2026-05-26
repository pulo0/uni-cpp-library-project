#include "../../include/pages/MainWindow.h"

#include "../../include/user/Admin.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    router_ = new AppRouter(stack_, this);

    build_pages();
    proceed_to_login();
}


void MainWindow::build_pages() {
    login_page_ = new LoginPage(this);
    router_->registerPage(Route::LoginPage, login_page_);
    connect(login_page_, &LoginPage::login_success, this, &MainWindow::on_login_success);

    user_menu_page_ = new UserMenuPage(this);
    router_->registerPage(Route::UserMain, user_menu_page_);
    connect(user_menu_page_, &UserMenuPage::go_available, router_, [this] {
        router_->go(Route::UserAvailable);
    });
    connect(user_menu_page_, &UserMenuPage::go_rented, router_, [this] {
        user_rented_page_->refresh();
        router_->go(Route::UserRented);
    });
    connect(user_menu_page_, &UserMenuPage::logout, this, &MainWindow::on_logout);

    user_available_page_ = new UserAvailablePage(this);
    router_->registerPage(Route::UserAvailable, user_available_page_);
    connect(user_available_page_, &UserAvailablePage::back, router_, [this] {
        router_->go(Route::UserMain);
    });

    user_rented_page_ = new UserRentedPage(this);
    router_->registerPage(Route::UserRented, user_rented_page_);
    connect(user_rented_page_, &UserRentedPage::back, router_, [this] {
        router_->go(Route::UserMain);
    });

    admin_page_ = new AdminPage(this);
    router_->registerPage(Route::AdminPage, admin_page_);
    connect(admin_page_, &AdminPage::logout, this, &MainWindow::on_logout);
}

void MainWindow::proceed_after_login() const {
    if (!current_user_) {
        proceed_to_login();
        return;
    }

    if (dynamic_cast<Admin *>(current_user_.get()) != nullptr) {
        admin_page_->refresh();
        router_->go(Route::AdminPage);
    } else {
        user_available_page_->refresh();
        user_rented_page_->refresh();
        router_->go(Route::UserMain);
    }
}

void MainWindow::on_login_success(UserBase *user) {
    current_user_.reset(user);

    user_menu_page_->set_user(current_user_.get());
    user_available_page_->set_user(current_user_.get());
    user_rented_page_->set_user(current_user_.get());
    admin_page_->set_user(current_user_.get());

    proceed_after_login();
}

void MainWindow::proceed_to_login() const {
    router_->go(Route::LoginPage);
}

void MainWindow::on_logout() {
    current_user_.reset();
    proceed_to_login();
}
