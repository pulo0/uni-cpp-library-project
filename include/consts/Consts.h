#ifndef LIBRARY_PROJECT_CONSTS_H
#define LIBRARY_PROJECT_CONSTS_H
#include <string>

namespace consts {
    // string_view because it can point at string literal during compile time
    // string allocates memory dynamically
    inline constexpr std::string_view db_name = "main.sqlite";
    inline constexpr std::string_view init_db_name = "library_sqlite_init.sql";
}

#endif
