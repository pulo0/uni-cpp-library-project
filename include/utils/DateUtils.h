#ifndef LIBRARY_PROJECT_DATEUTILS_H
#define LIBRARY_PROJECT_DATEUTILS_H
#include <ctime>
#include <stdexcept>

namespace date_utils {
    std::string to_iso8601_utc(time_t t);
    time_t from_iso8601_utc(const std::string &s);
}

#endif