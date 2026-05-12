#include "../../include/db/DBManager.h"

#include <fstream>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

const std::string file_path = "../../data/library_sqlite_init.sql";

static std::string load_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot open SQL file");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

DBManager &DBManager::getInstance() {
    static DBManager instance;
    return instance;
}

bool DBManager::connect(const std::string &db_path) {
    if (db_) return true;
    try {
        if (sqlite3_open(db_path.c_str(), &db_) != SQLITE_OK) {
            throw std::runtime_error("Failed to open SQLite database");
        }
        const std::string sql = load_file(file_path);
        char *err = nullptr;
        if (const int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err); rc != SQLITE_OK) {
            const std::string msg = err != nullptr ? err : sqlite3_errmsg(db_);
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }
        return true;
    } catch (...) {
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
