#ifndef LIBRARY_PROJECT_BOOK_H
#define LIBRARY_PROJECT_BOOK_H
#include <string>

enum class CoverType { Paperback, Hardcover, Unknown };

static std::string cover_translate(const CoverType &cover) {
    switch (cover) {
        case CoverType::Paperback:
            return "paperback";
        case CoverType::Hardcover:
            return "hardcover";
        case CoverType::Unknown:
            break;
    }
    return "unknown";
}

class Book {
public:
    int id;
    std::string title;
    std::string author;
    std::string description;
    int year_of_publish;
    CoverType cover;
    std::string return_date;
    bool is_available;
    bool is_prolonged;
};

#endif
