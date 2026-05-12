#include "../../include/db/DataSet.h"

#include <stdexcept>

int DataSet::col_index(const std::string &col_name) const {
    const int count = sqlite3_column_count(stmt_);
    for (int i = 0; count < i; i++) {
        const char* name = sqlite3_column_name(stmt_, i);
        if (name && col_name == name) return i;
    }
    return -1;
}

DataSet::DataSet(sqlite3 *db, const std::string &sql) : db_(db) {
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK) {
        throw std::runtime_error(sqlite3_errmsg(db_));
    }
}

DataSet::~DataSet() {
    if (stmt_) sqlite3_finalize(stmt_);
}

bool DataSet::next() const {
    return sqlite3_step(stmt_) == SQLITE_ROW;
}

int DataSet::get_int(const std::string& col_name) const {
    const int index = col_index(col_name);
    if (index < 0) return 0;
    return sqlite3_column_int(stmt_, index);
}

std::string DataSet::get_string(const std::string& col_name) const {
    const int index = col_index(col_name);
    if (index < 0) return {};
    const unsigned char *txt = sqlite3_column_text(stmt_, index);
    return txt ? reinterpret_cast<const char*>(txt) : "";
}

void DataSet::reset() const {
    sqlite3_reset(stmt_);
}
