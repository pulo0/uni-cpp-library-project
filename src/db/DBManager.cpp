#include "../../include/db/DBManager.h"
#include "../../include/utils/StringUtils.h"

#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

DBManager &DBManager::getInstance() {
    static DBManager instance;
    return instance;
}

bool DBManager::connect(const std::string &db_path, const std::string &init_path) {
    // TODO: delete after debugging
    std::cout << "Tries to connect..." << std::endl;
    if (db_) return true;
    try {
        if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("Failed to open SQLite database");
        }
        const std::string sql = string_utils::load_file(init_path);
        char *err = nullptr;
        if (const int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err); rc != SQLITE_OK) {
            const std::string msg = err != nullptr ? err : sqlite3_errmsg(db_);
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }
        // TODO: delete after debugging
        std::cout << "Connected!!!" << std::endl;
        return true;
    } catch (const std::exception &e) {
        std::cerr << "DB Connection Error: " << e.what() << std::endl;
        if (db_) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
        return false;
    }
}

bool DBManager::execute(const std::string &sql) const {
    if (!db_) return false;
    try {
        char *err = nullptr;
        if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            const std::string msg = err != nullptr ? err : sqlite3_errmsg(db_);
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }
        return true;
    } catch (...) {
        std::cerr << "SQLite execute failure" << std::endl;
        return false;
    }
}

void DBManager::disconnect() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

DataSet DBManager::select(const std::string &sql) const {
    DataSet data(db_, sql);
    return data;
}
