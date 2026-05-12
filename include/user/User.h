#ifndef LIBRARY_PROJECT_USER_H
#define LIBRARY_PROJECT_USER_H
#include "UserBase.h"
#include "../structs/Book.h"

class User : public UserBase {
public:
    void borrow_book(Book *book);

    void return_book(Book *book);

    void prolong_book(Book *book);

    void settle_fees();
};

#endif
