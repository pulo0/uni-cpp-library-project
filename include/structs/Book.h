#ifndef LIBRARY_PROJECT_BOOK_H
#define LIBRARY_PROJECT_BOOK_H
#include <string>

enum CoverType { Paperback, Hardcover, Unknown };

class Book {
public:
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
