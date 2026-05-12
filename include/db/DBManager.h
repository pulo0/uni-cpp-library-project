#ifndef LIBRARY_PROJECT_DBMANAGER_H
#define LIBRARY_PROJECT_DBMANAGER_H
#include <sqlite3.h>
#include <string>

#include "DataSet.h"

class DBManager {
public:
    static DBManager &getInstance();

    bool connect(const std::string &db_path);

    void disconnect();

    bool execute(const std::string& sql) const;

    DataSet select(const std::string& sql);

    sqlite3 *raw() const { return db_; }

private:
    sqlite3 *db_ = nullptr;
};

#endif
