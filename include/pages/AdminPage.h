#ifndef LIBRARY_PROJECT_ADMINPAGE_H
#define LIBRARY_PROJECT_ADMINPAGE_H
#include <QTableWidget>
#include <QPushButton>

#include "../structs/Book.h"
#include "../user/UserBase.h"

class AdminPage : public QWidget {
    Q_OBJECT

public:
    // constructor with parameter necessary for QT Widgets
    explicit AdminPage(QWidget *parent = nullptr);

    // setter and refresh function to reload the state of data on page
    void set_user(UserBase *user);

    void refresh();

signals:
    void logout();

private slots:
    void on_add_book();

    void on_logout_clicked();

private:
    void setup_ui();

    void fill_table();

    void add_row(const Book &book, int row, bool is_rented, int client_id, const QString &client_login,
                 const QString &due_date);

    // properties for persistence for site
    UserBase *user_ = nullptr;
    std::vector<Book> books_;

    // widgets used throughout the page
    QTableWidget *table_ = nullptr;
    QPushButton *add_btn_ = nullptr;
    QPushButton *logout_btn_ = nullptr;
};

#endif
