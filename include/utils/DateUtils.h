#ifndef LIBRARY_PROJECT_DATEUTILS_H
#define LIBRARY_PROJECT_DATEUTILS_H
#include <ctime>
#include <qstring.h>
#include <stdexcept>

namespace date_utils {
    QString ymd_date(const std::string &iso8601);
    std::string to_iso8601_utc(time_t t);
    time_t from_iso8601_utc(const std::string &s);
}

#endif