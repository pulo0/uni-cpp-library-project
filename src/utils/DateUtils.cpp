#include "../../include/utils/DateUtils.h"

namespace date_utils {
    std::string to_iso8601_utc(time_t t) {
        std::tm tm{};
#if defined(_WIN32)
        gmtime_s(&tm, &t);
#else
        gmtime_r(&t, &tm);
#endif
        char buf[21];
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
        auto buf_to_str = std::string(buf);
        return buf_to_str;
    }

    time_t from_iso8601_utc(const std::string &s) {
        // format check from ISO8601: YYYY-MM-DDTHH:MM:SSZ where Z is
        if (s.size() != 20 || s[4] != '-' || s[7] != '-'
            || s[10] != 'T' || s[13] != ':' || s[16] != ':' || s[19] != 'Z') {
            throw std::runtime_error("Bad timestamp format");
        }

        std::tm tm{};

        // conversion to tm unit (tm_year = 0 => 1900 year)
        tm.tm_year = std::stoi(s.substr(0, 4)) - 1900;
        // conversion to tm unit (tm_mon = 0 => January)
        tm.tm_mon = std::stoi(s.substr(5, 2)) - 1;
        tm.tm_mday = std::stoi(s.substr(8, 2));
        tm.tm_hour = std::stoi(s.substr(11, 2));
        tm.tm_min = std::stoi(s.substr(14, 2));
        tm.tm_sec = std::stoi(s.substr(17, 2));

#if defined(_WIN32)
        return _mkgmtime(&tm);
#else
        return timegm(&tm);
#endif
    }
}
