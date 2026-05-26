#include "../../include/pages/UserAvailablePage.h"
#include "../../include/user/User.h"
#include <iostream>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>

#include "../../include/db/DBManager.h"


UserAvailablePage::UserAvailablePage(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

void UserAvailablePage::setup_ui() {
    table_ = new QTableWidget(this);
    table_->setColumnCount(5);
    table_->setHorizontalHeaderLabels({
        "Title", "Author", "Year of Publish", "Cover", "Borrow"
    });
    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionMode(QAbstractItemView::NoSelection);

    back_btn_ = new QPushButton("Cancel", this);

    auto *bottom = new QHBoxLayout;
    bottom->addStretch();
    bottom->addWidget(back_btn_);

    auto *root = new QVBoxLayout(this);
    root->addWidget(table_);
    root->addLayout(bottom);

    connect(back_btn_, &QPushButton::clicked, this, &UserAvailablePage::back);
}

void UserAvailablePage::refresh() {
    if (const auto *usr = dynamic_cast<User *>(user_); !usr) return;

    const auto &db = DBManager::getInstance();
    const std::string sql =
            "SELECT id, title, author, description, year_of_publish, cover, is_available "
            "FROM Books WHERE is_available = 1;";

    auto rows = db.select(sql);

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
        b.is_available = rows.get_int("is_available");

        books_.push_back(b);
        table_->insertRow(row_index);
        add_row(row_index, b);
        ++row_index;
    }
}

void UserAvailablePage::add_row(const int row, const Book &book) {
    const QString cover_str = QString::fromStdString(cover_translate(book.cover));
    table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book.title)));
    table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.author)));
    table_->setItem(row, 2, new QTableWidgetItem(QString::number(book.year_of_publish)));
    table_->setItem(row, 3, new QTableWidgetItem(cover_str));

    auto *borrow_btn = new QPushButton("Borrow", this);
    table_->setCellWidget(row, 4, borrow_btn);

    connect(borrow_btn, &QPushButton::clicked, this, [this, book] {
        const auto *usr = dynamic_cast<User *>(user_);
        if (!usr) return;

        try {
            usr->borrow_book(&book);
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Book borrow failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Book borrow failed", exc.what());
        }
    });
}

void UserAvailablePage::set_user(UserBase *user) {
    user_ = user;
}
