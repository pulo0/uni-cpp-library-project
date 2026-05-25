#include <fstream>
#include <sstream>

namespace string_utils {
    /// Helper function for loading file's entire content (.rdbuf())
    std::string load_file(const std::string &path) {
        std::ifstream file(path);
        if (!file) throw std::runtime_error("Cannot open a file");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
