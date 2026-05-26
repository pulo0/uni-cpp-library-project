#ifndef LIBRARY_PROJECT_USER_H
#define LIBRARY_PROJECT_USER_H
#include "UserBase.h"
#include "../structs/Book.h"

class User : public UserBase {
public:
    void borrow_book(const Book *book) const;

    void return_book(const Book *book);

    void prolong_book(const Book *book);

    // void settle_fees();
};

#endif
