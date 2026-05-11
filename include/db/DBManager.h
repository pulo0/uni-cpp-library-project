#ifndef LIBRARY_PROJECT_DBMANAGER_H
#define LIBRARY_PROJECT_DBMANAGER_H
#include <string>

class DBManager {
public:
    static DBManager& getInstance();
    bool connect(std::string db_path);
    void disconnect();
    bool execute(std::string sql);
};

#endif