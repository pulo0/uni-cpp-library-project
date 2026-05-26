#ifndef LIBRARY_PROJECT_USERRENTEDPAGE_H
#define LIBRARY_PROJECT_USERRENTEDPAGE_H
#include <QPushButton>
#include <QTableWidget>
#include <QWidget>
#include "../structs/Book.h"
#include "../user/UserBase.h"

class UserRentedPage : public QWidget {
    Q_OBJECT;

public:
    explicit UserRentedPage(QWidget *parent = nullptr);

    void set_user(UserBase *user);

    void refresh();

signals:
    void back();

private:
    void setup_ui();

    void add_row(int row, const Book &book, bool is_prolonged);

    UserBase *user_ = nullptr;
    std::vector<Book> books_;

    QTableWidget *table_ = nullptr;
    QPushButton *back_btn_ = nullptr;
};

#endif
