#include "../../include/user/User.h"
#include "../../include/db/DBManager.h"
#include "../../include/utils/DateUtils.h"
#include "../../include/utils/SqlUtils.h"
#include <ctime>
#include <iostream>

std::string generate_due_date(const time_t epoch) {
    auto tm = *std::localtime(&epoch);
    tm.tm_mon++;
    return date_utils::to_iso8601_utc(mktime(&tm));
}


void User::borrow_book(const Book *book) const {
    const auto &db = DBManager::getInstance();

    const std::string gen_date = generate_due_date(std::time(nullptr));

    const std::string update_sql =
            "UPDATE Books SET is_available = 0 WHERE id = " + std::to_string(book->id) + ";";

    const std::string rental_sql = "INSERT INTO Rental (id_book, id_user, due_date, is_prolonged) VALUES("
                                   + std::to_string(book->id) + ", " + std::to_string(this->id) + ", " +
                                   sql_utils::literal(gen_date) + ", 0);";

    // Execute transaction part, omit bool type of db.execute part for IDE to not yell (it doesn't require)
    (void) db.execute("BEGIN TRANSACTION;");
    try {
        if (!db.execute(rental_sql)) {
            throw std::runtime_error("Unable to insert new rental (borrow a book)");
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

void User::return_book(const Book *book) {
    const auto &db = DBManager::getInstance();

    const std::string update_sql = "UPDATE Books SET is_available = 1 WHERE id = " + std::to_string(book->id) + ";";

    const std::string delete_rent_sql = "DELETE FROM Rental WHERE id_book = " + std::to_string(book->id) + ";";

    (void) db.execute("BEGIN TRANSACTION;");
    try {
        if (!db.execute(delete_rent_sql)) {
            throw std::runtime_error("Unable to delete rental (return a book)");
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

void User::prolong_book(const Book *book) {
    const auto &db = DBManager::getInstance();

    const time_t due_date = date_utils::from_iso8601_utc(book->return_date);
    const std::string prolong_date = generate_due_date(due_date);

    const std::string update_sql = "UPDATE Rental SET "
                                   "due_date = " + sql_utils::literal(prolong_date) + ", is_prolonged = 1"
                                   " WHERE id_book = " + std::to_string(book->id) + " AND is_prolonged = 0;";

    if (!db.execute(update_sql)) {
        throw std::runtime_error("Unable to prolong a book for the user");
    }
}
