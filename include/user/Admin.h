#ifndef LIBRARY_PROJECT_ADMIN_H
#define LIBRARY_PROJECT_ADMIN_H
#include "User.h"
#include "UserBase.h"
#include "../structs/Book.h"

class Admin : public UserBase {
public:
    void add_book(const std::string &title, const std::string &author, const std::string &description, int year_of_publish, CoverType cover);

    void delete_book(const Book *book);

    void edit_book(const Book *book, const std::string &title, const std::string &author, const std::string &description, int year_of_publish,
                   CoverType cover);

    void revoke_book(const Book *book, const User *user);
};

#endif
