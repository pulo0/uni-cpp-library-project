#ifndef LIBRARY_PROJECT_SQLUTILS_H
#define LIBRARY_PROJECT_SQLUTILS_H
#include <string>
#include "../../include/db/DBManager.h"

namespace sql_utils {
    std::string sql_literal(const std::string &s);
    void insert_mock_data(const DBManager &manager);
}

#endif