#ifndef LIBRARY_PROJECT_DATASET_H
#define LIBRARY_PROJECT_DATASET_H
#include <sqlite3.h>
#include <string>

class DataSet {
public:
    DataSet(sqlite3 *db, const std::string &sql);

    ~DataSet();

    bool next() const;

    std::string get_string(const std::string &col_name) const;

    int get_int(const std::string &col_name) const;

    void reset() const;

private:
    int col_index(const std::string &col_name) const;

    sqlite3 *db_ = nullptr;
    sqlite3_stmt *stmt_ = nullptr;
};

#endif
