#include "../../include/pages/AdminPage.h"

#include <iostream>
#include <qboxlayout.h>
#include <QHeaderView>
#include <QMessageBox>

#include "../../include/db/DBManager.h"
#include "../../include/dialogs/BookDialog.h"
#include "../../include/user/Admin.h"
#include "../../include/utils/DateUtils.h"

AdminPage::AdminPage(QWidget *parent) : QWidget(parent) {
    setup_ui();
}

void AdminPage::set_user(UserBase *user) {
    user_ = user;
}

void AdminPage::setup_ui() {
    table_ = new QTableWidget(this);
    table_->setColumnCount(9);
    table_->setHorizontalHeaderLabels({
        "Title", "Author", "Year", "Available",
        "Client", "Due date", "Edit", "Delete", "Revoke"
    });

    table_->horizontalHeader()->setStretchLastSection(true);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionMode(QAbstractItemView::NoSelection);
    table_->setAlternatingRowColors(true);

    add_btn_ = new QPushButton("Add book", this);
    logout_btn_ = new QPushButton("Log out", this);

    auto *bottom = new QHBoxLayout;
    bottom->addWidget(add_btn_);
    bottom->addStretch();
    bottom->addWidget(logout_btn_);

    auto *root = new QVBoxLayout(this);
    root->addWidget(table_);
    root->addLayout(bottom);

    connect(add_btn_, &QPushButton::clicked, this, &AdminPage::on_add_book);
    connect(logout_btn_, &QPushButton::clicked, this, &AdminPage::on_logout_clicked);
}

void AdminPage::on_logout_clicked() {
    emit logout();
}

void AdminPage::on_add_book() {
    auto *admin = dynamic_cast<Admin *>(user_);
    if (!admin) return;

    BookDialog dlg(this);
    dlg.setWindowTitle("Add book");

    if (dlg.exec() != QDialog::Accepted) return;

    try {
        admin->add_book(dlg.title(), dlg.author(), dlg.description(), dlg.year_of_publish(), dlg.cover());
        refresh();
    } catch (const std::exception &exc) {
        std::cerr << "Book addition failed: " << exc.what() << std::endl;
        QMessageBox::warning(this, "Book addition failed", exc.what());
    }
}

void AdminPage::refresh() {
    if (!user_) return;
    if (dynamic_cast<Admin *>(user_) == nullptr) return;

    const auto &db = DBManager::getInstance();

    const std::string select_sql =
            "SELECT "
            "Books.id, Books.title, Books.author, Books.description, Books.year_of_publish, "
            "Books.cover, Books.is_available, Users.id as client_id, Users.login as client_login, "
            "Rental.due_date "
            "FROM Books "
            "LEFT JOIN Rental ON Rental.id_book = Books.id "
            "LEFT JOIN Users ON Users.id = Rental.id_user;";

    const auto rows = db.select(select_sql);

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

        const int client_id = rows.get_int("client_id");
        const QString client_login = QString::fromStdString(rows.get_string("client_login"));
        const QString due_date = date_utils::ymd_date(rows.get_string("due_date"));

        const bool is_rented = b.is_available == 0;

        books_.push_back(b);
        table_->insertRow(row_index);
        add_row(b, row_index, is_rented, client_id, client_login, due_date);
        ++row_index;
    }
}

void AdminPage::add_row(const Book &book, const int row, const bool is_rented, int client_id,
                        const QString &client_login,
                        const QString &due_date) {
    // Book data knowledge (from column "title" to "Due date")
    table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book.title)));
    table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.author)));
    table_->setItem(row, 2, new QTableWidgetItem(QString::number(book.year_of_publish)));
    table_->setItem(row, 3, new QTableWidgetItem(book.is_available ? "Yes" : "No"));
    table_->setItem(row, 4, new QTableWidgetItem(is_rented ? client_login : "-"));
    table_->setItem(row, 5, new QTableWidgetItem(is_rented ? due_date : "-"));

    // Three button layout (edit, delete, revoke respectively)
    auto *edit_btn = new QPushButton("Edit", this);
    table_->setCellWidget(row, 6, edit_btn);

    connect(edit_btn, &QPushButton::clicked, this, [this, book] {
        auto *admin = dynamic_cast<Admin *>(user_);
        if (!admin) return;

        BookDialog dlg(book, this);
        dlg.setWindowTitle("Edit book");

        if (dlg.exec() != QDialog::Accepted) return;

        try {
            admin->edit_book(&book, dlg.title(), dlg.author(), dlg.description(), dlg.year_of_publish(), dlg.cover());
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Book edit failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Book edit failed", exc.what());
        }
    });

    auto *del_btn = new QPushButton("Delete", this);
    table_->setCellWidget(row, 7, del_btn);

    connect(del_btn, &QPushButton::clicked, this, [this, book] {
        auto *admin = dynamic_cast<Admin *>(user_);
        if (!admin) return;

        const QString msg_desc =
                "Are you sure you want to delete this book with "
                "its rental connections automatically revoking access "
                "to it for the certain user?";

        if (QMessageBox::question(this, "Delete book",
                                  msg_desc)
            != QMessageBox::Yes) {
            return;
        }

        try {
            admin->delete_book(&book);
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Book deletion failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Book deletion failed", exc.what());
        }
    });

    auto *revoke_btn = new QPushButton("Revoke", this);
    table_->setCellWidget(row, 8, revoke_btn);
    revoke_btn->setEnabled(is_rented);

    connect(revoke_btn, &QPushButton::clicked, this, [this, book, client_id, client_login] {
        auto *admin = dynamic_cast<Admin *>(user_);
        if (!admin) return;

        if (client_id <= 0) {
            QMessageBox::information(this, "Revoke", "No client for this book");
            return;
        }

        if (const QString msg_desc = "Are sure you want to revoke user's (" + client_login + ") book?";
            QMessageBox::question(this, "Revoke book", msg_desc) != QMessageBox::Yes) {
            return;
        }

        try {
            User user{};
            user.id = client_id;
            admin->revoke_book(&book, &user);
            refresh();
        } catch (const std::exception &exc) {
            std::cerr << "Revoke failed: " << exc.what() << std::endl;
            QMessageBox::warning(this, "Revoke failed", exc.what());
        }
    });
}
