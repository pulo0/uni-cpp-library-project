#ifndef LIBRARY_PROJECT_DBMANAGER_H
#define LIBRARY_PROJECT_DBMANAGER_H
#include <sqlite3.h>
#include <string>

#include "DataSet.h"

class DBManager {
public:
    static DBManager &getInstance();

    bool connect(const std::string &db_path, const std::string &init_path);

    void disconnect();

    [[nodiscard]] bool execute(const std::string& sql) const;

    [[nodiscard]] inline DataSet select(const std::string& sql) const;

    [[nodiscard]] sqlite3 *raw() const { return db_; }

private:
    sqlite3 *db_ = nullptr;
};

#endif
