cmake_minimum_required(VERSION 4.1)
project(library_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)

find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(library_project main.cpp
        include/db/DBManager.h
        include/db/DataSet.h
        include/user/UserBase.h
        include/user/User.h
        include/user/Admin.h
        include/structs/Book.h
        include/auth/Authentication.h
        src/db/DBManager.cpp
        src/db/DataSet.cpp
        include/router/AppRouter.h
        src/router/AppRouter.cpp)

target_compile_definitions(library_project PRIVATE PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(library_project PRIVATE
        SQLite::SQLite3
        Qt6::Widgets)