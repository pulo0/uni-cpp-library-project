#ifndef LIBRARY_PROJECT_ADMIN_H
#define LIBRARY_PROJECT_ADMIN_H
#include "User.h"
#include "UserBase.h"
#include "../structs/Book.h"

class Admin : public UserBase {
public:
    void add_book(std::string title, std::string author, std::string description, int year_of_publish, CoverType cover);
    void delete_book(Book* book);
    void edit_book(Book *book, std::string title, std::string author, std::string description, int year_of_publish,
                   CoverType cover);
    void revoke_book(Book* book, User* user);
};

#endif
