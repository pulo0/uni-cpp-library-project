#ifndef LIBRARY_PROJECT_AUTHENTICATION_H
#define LIBRARY_PROJECT_AUTHENTICATION_H
#include <memory>

#include "../user/UserBase.h"

class Authentication {
public:
    std::unique_ptr<UserBase> login(std::string login, std::string password);

    std::unique_ptr<UserBase> register_user(std::string login, std::string password);

    void logout(std::unique_ptr<UserBase> &current_user);

    void forgot_password(const std::string& login, const std::string& new_password, const std::string &new_password_confirm);
};

#endif
