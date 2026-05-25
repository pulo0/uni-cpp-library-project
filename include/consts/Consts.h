#ifndef LIBRARY_PROJECT_CONSTS_H
#define LIBRARY_PROJECT_CONSTS_H
#include <string>

namespace consts {
    inline constexpr std::string_view db_name = "main.sqlite";
    inline constexpr std::string_view init_db_name = "library_sqlite_init.sql";
}

#endif