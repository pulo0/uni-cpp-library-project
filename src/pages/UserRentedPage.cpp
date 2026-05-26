#include "../../include/pages/UserRentedPage.h"

#include <iostream>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

#include "../../include/db/DBManager.h"
#include "../../include/user/User.h"
#include "../../include/utils/DateUtils.h"

UserRentedPage::UserRentedPage(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

void UserRentedPage::set_user(UserBase *user) {
    user_ = user;
}

void UserRentedPage::setup_ui() {
    table_ = new QTableWidget(this);
    table_->setColumnCount(7);
    table_->setHorizontalHeaderLabels({
        "Title", "Author", "Cover", "Prolonged", "Due date", "Prolong", "Return"
    });
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionMode(QAbstractItemView::NoSelection);
    table_->setAlternatingRowColors(true);

    back_btn_ = new QPushButton("Cancel", this);

    auto *bottom = new QHBoxLayout;
    bottom->addStretch();
    bottom->addWidget(back_btn_);

    auto *root = new QVBoxLayout(this);
    root->addWidget(table_);
    root->addLayout(bottom);

    connect(back_btn_, &QPushButton::clicked, this, &UserRentedPage::back);
}

void UserRentedPage::refresh() {
    const auto *u = dynamic_cast<User *>(user_);
    if (!u) return;

    const auto &db = DBManager::getInstance();

    const std::string sql =
            "SELECT "
            "Books.id, Books.title, Books.author, Books.description, Books.year_of_publish, Books.cover, "
            "Rental.due_date, Rental.is_prolonged "
            "FROM Rental "
            "JOIN Books ON Books.id = Rental.id_book "
            "WHERE Rental.id_user = " + std::to_string(u->id) + ";";

    const auto rows = db.select(sql);

    books_.clear();
    table_->setRowCount(0);

    int row_index = 0;
    while (rows.next()) {
        Book b{};
        b.id = rows.get_int("id");
        b.title = rows.get_string("title");
        b.author = rows.get_string("author");
        b.description = rows.get_string("description");
        b.year_of_publish = rows.get_int("year_of_publish");
        b.cover = to_cover_translate(rows.get_string("cover"));
        b.is_available = false;
        b.return_date = rows.get_string("due_date");
        const bool is_prolonged = rows.get_int("is_prolonged") != false;

        books_.push_back(b);
        table_->insertRow(row_index);
        add_row(row_index, b, is_prolonged);
        ++row_index;
    }
}

void UserRentedPage::add_row(const int row, const Book &book, const bool is_prolonged) {
    table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book.title)));
    table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.author)));
    table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(cover_translate(book.cover))));
    table_->setItem(row, 3, new QTableWidgetItem(is_prolonged ? "Yes" : "No"));
    table_->setItem(row, 4, new QTableWidgetItem(date_utils::ymd_date(book.return_date)));

    auto *prolong_btn = new QPushButton("Prolong", this);
    table_->setCellWidget(row, 5, prolong_btn);
    prolong_btn->setEnabled(!is_prolonged);

    connect(prolong_btn, &QPushButton::clicked, this, [this, book] {
        auto *usr = dynamic_cast<User *>(user_);
        if (!usr) return;

        try {
            usr->prolong_book(&book);
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Book prolong failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Book prolong failed", exc.what());
        }
    });

    auto *ret_btn = new QPushButton("Return", this);
    table_->setCellWidget(row, 6, ret_btn);

    connect(ret_btn, &QPushButton::clicked, this, [this, book] {
        auto *usr = dynamic_cast<User *>(user_);
        if (!usr) return;

        try {
            usr->return_book(&book);
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Book return failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Book return failed", exc.what());
        }
    });
}
