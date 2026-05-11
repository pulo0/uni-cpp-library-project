#ifndef LIBRARY_PROJECT_DATASET_H
#define LIBRARY_PROJECT_DATASET_H
#include <string>

class DataSet {
public:
    bool next();
    std::string get_string(std::string col_name);
    int get_int(std::string col_name);
    void reset();
};

#endif