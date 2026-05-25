#include "../../include/auth/Authentication.h"

#include <algorithm>

#include "../../include/db/DBManager.h"
#include "../../include/user/Admin.h"
#include "../../include/utils/SqlUtils.h"

/// Login logic for connectivity with the SQL and check for the password (not hashed)
/// Checks for:
/// - whether next row exists (meaning first row altogether), if not then login is invalid (doesn't exist in DB)
/// - correctness between user's input and password stored on the DB, if not then the password is invalid.
///
/// After the process went correctly, pointer of UserBase will be directed to either Admin or User new instance
/// (dependable on is_admin bool)
std::unique_ptr<UserBase> Authentication::login(std::string login, std::string password) {
    const auto &db = DBManager::getInstance();

    const std::string sql =
            "SELECT login, password, is_admin"
            "FROM Users"
            "WHERE login =" + sql_utils::sql_literal(login);

    const auto rows = db.select(sql);
    if (!rows.next())
        throw std::runtime_error("Invalid login");

    if (const std::string db_password = rows.get_string("password"); db_password != password)
        throw std::runtime_error("Invalid password");

    const bool is_admin = rows.get_int("is_admin");

    std::unique_ptr<UserBase> user;
    if (is_admin) user = std::make_unique<Admin>();
    else user = std::make_unique<User>();

    user->login = std::move(login);
    user->password = std::move(password);
    return user;
}

/// Executes a INSERT INTO query to DB with user's input data and returns login information
/// (created pointer as well)
std::unique_ptr<UserBase> Authentication::register_user(std::string login, std::string password) {
    const auto &db = DBManager::getInstance();

    const std::string sql =
            "INSERT INTO Users (login password is_admin) VALUES ("
            + sql_utils::sql_literal(login) + ", " + sql_utils::sql_literal(password) + ", 0);";

    if (!db.execute(sql))
        throw std::runtime_error("DB Insertion Failed: Unable to register a new user");

    return this->login(std::move(login), std::move(password));
}

void Authentication::forgot_password(const std::string &login, const std::string &new_password,
                                     const std::string &new_password_confirm) {
    const auto &db = DBManager::getInstance();

    if (new_password != new_password_confirm) {
        throw std::runtime_error("Password are invalid to one another");
    }

    const std::string check_sql =
            "SELECT * FROM Users WHERE login = " + sql_utils::sql_literal(login) + "LIMIT 1;";
    if (const auto rows = db.select(check_sql); !rows.next()) {
        throw std::runtime_error("No available login user to change the password");
    }

    const std::string update_sql = "UPDATE Users SET password ="
                                   + sql_utils::sql_literal(new_password) + " WHERE login = " + sql_utils::sql_literal(login) +
                                   ";";

    if (!db.execute(update_sql)) {
        throw std::runtime_error("Unable to reset forgotten password by the user");
    }
}

void Authentication::logout(std::unique_ptr<UserBase> &current_user) {
    current_user.reset();
}
