#include "../../include/utils/StringUtils.h"
#include "../../include/db/DBManager.h"
#include <string>
#include <filesystem>

namespace sql_utils {
    /// Helper function providing a SQL literal for variable
    /// (e.x. 'Adam' or when there's a string like O'Brian, SQL usually uses double ' to escape the character)
    std::string literal(const std::string &s) {
        std::string out = "'";
        for (const char character: s) {
            out += character == '\'' ? "''" : std::string(1, character);
        }
        out += "'";
        return out;
    }

    void insert_mock_data(const DBManager &manager) {
        namespace fs = std::filesystem;
        const fs::path mock_data = fs::path(PROJECT_ROOT_DIR) / "data" / "library_mock_data.sql";

        char *err = nullptr;
        if (const int rc = sqlite3_exec(manager.raw(), string_utils::load_file(mock_data).c_str(), nullptr, nullptr,
                                        &err);
            rc != SQLITE_OK) {
            const std::string msg = err != nullptr ? err : sqlite3_errmsg(manager.raw());
            sqlite3_free(err);
            throw std::runtime_error(msg);
        }
    }
}
