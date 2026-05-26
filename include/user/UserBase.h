#ifndef LIBRARY_PROJECT_USERBASE_H
#define LIBRARY_PROJECT_USERBASE_H
#include <string>

class UserBase {
public:
    int id;
    std::string login;
    std::string password;

    virtual ~UserBase();
};

#endif
