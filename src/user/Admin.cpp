#include "../../include/user/Admin.h"

#include <stdexcept>
#include <iostream>

#include "../../include/db/DBManager.h"
#include "../../include/utils/SqlUtils.h"

void Admin::add_book(const std::string &title, const std::string &author, const std::string &description,
                     const int year_of_publish,
                     const CoverType cover) {
    const auto &db = DBManager::getInstance();

    const std::string insert_sql =
            "INSERT INTO Books (title, author, description, year_of_publish, cover, is_available) VALUES("
            + sql_utils::literal(title) + ", " + sql_utils::literal(author) + ", " +
            sql_utils::literal(description) + ", "
            + std::to_string(year_of_publish) + ", " + sql_utils::literal(cover_translate(cover)) + ", 1);";

    if (!db.execute(insert_sql)) {
        throw std::runtime_error("Unable to add a book");
    }
}

void Admin::delete_book(const Book *book) {
    const auto &db = DBManager::getInstance();

    const std::string delete_rent_sql =
        "DELETE FROM Rental WHERE id_book = " + std::to_string(book->id) + ";";

    const std::string delete_book_sql =
            "DELETE FROM Books WHERE id = " + std::to_string(book->id) + ";";

    (void) db.execute("BEGIN TRANSACTION;");
    try {
        if (!db.execute(delete_rent_sql)) {
            throw std::runtime_error("Unable to delete rental entry while deleting book entry");
        }

        if (!db.execute(delete_book_sql)) {
            throw std::runtime_error("Unable to delete the book entry");
        }
        (void) db.execute("COMMIT;");
    } catch (const std::exception &exc) {
        (void) db.execute("ROLLBACK;");
        std::cerr << "Transaction failed: " << exc.what() << std::endl;
        throw;
    }
}

void Admin::edit_book(const Book *book, const std::string &title, const std::string &author,
                      const std::string &description, const int year_of_publish,
                      const CoverType cover) {
    const auto &db = DBManager::getInstance();

    const std::string update_sql =
            "UPDATE Books SET "
            "title = " + sql_utils::literal(title) + ", author = " + sql_utils::literal(author) + ", description = " +
            sql_utils::literal(description) + ", year_of_publish = " + std::to_string(year_of_publish) + ", cover = " +
            sql_utils::literal(cover_translate(cover)) + " WHERE id = " + std::to_string(book->id) + ";";

    if (!db.execute(update_sql)) {
        throw std::runtime_error("Unable to update the pre-existing entry of a book");
    }
}

void Admin::revoke_book(const Book *book, const User *user) {
    const auto &db = DBManager::getInstance();

    const std::string delete_sql = "DELETE FROM Rental WHERE "
                                   "id_book = " + std::to_string(book->id) + " AND id_user = " + std::to_string(
                                       user->id) + ";";

    const std::string update_sql = "UPDATE Books SET is_available = 1 "
                                   "WHERE id = " + std::to_string(book->id) + ";";

    (void) db.execute("BEGIN TRANSACTION;");
    try {
        if (!db.execute(delete_sql)) {
            throw std::runtime_error("Unable to delete rental (revoke a book)");
        }

        if (!db.execute(update_sql)) {
            throw std::runtime_error("Unable to change status of the book");
        }
        (void) db.execute("COMMIT;");
    } catch (const std::exception &exc) {
        (void) db.execute("ROLLBACK;");
        std::cerr << "Transaction failed: " << exc.what() << std::endl;
        throw;
    }
}
