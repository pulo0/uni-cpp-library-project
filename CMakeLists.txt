cmake_minimum_required(VERSION 4.1)
project(library_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(library_project main.cpp
        include/db/DBManager.h
        include/db/DataSet.h
        include/user/UserBase.h
        include/user/User.h
        include/user/Admin.h
        include/structs/Book.h
        include/auth/Authentication.h)
