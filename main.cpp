#include <iostream>
#include <filesystem>
#include <ctime>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>

#include "include/db/DBManager.h"
#include "include/utils/SqlUtils.h"
#include "include/consts/Consts.h"
#include "include/utils/DateUtils.h"

/// Initializes connection between the DB.
///
/// File path to the main db is in build directory under the data folder (canonical so absolute path, from beginning).
///
/// It either creates or finds the directory and file under this directory and proceeds to find initialization file
/// from PROJECT_ROOT_DIR (pinpoints a project directory at the level where CMakeLists.txt file exist)
bool db_init(DBManager &manager, char *exe_path) {
    namespace fs = std::filesystem;
    const fs::path exec_dir = fs::canonical(fs::path(exe_path)).parent_path();
    const fs::path db_dir = exec_dir / "data";
    fs::create_directories(db_dir);

    const fs::path db_path = db_dir / consts::db_name;
    const fs::path init_path = fs::path(PROJECT_ROOT_DIR) / "data" / consts::init_db_name;

    if (!fs::exists(init_path)) {
        std::cerr << "Init File Not Found: " << init_path << std::endl;
        return false;
    }

    std::cout << "DB Directory: " << db_path.parent_path() << std::endl;

    if (!manager.connect(db_path.string(), init_path.string())) {
        std::cerr << "DB connect failed to: " << db_path << std::endl;
        return false;
    }

    // TODO: Uncomment only if you test quickly db and you want mock data
    // sql_utils::insert_mock_data(manager);
    return true;
}

int main(int argc, char *argv[]) {
    auto &db = DBManager::getInstance();

    if (!db_init(db, argv[0])) {
        return 1;
    }

    // TODO: make a proper router for all the pages to go smoothly
    QApplication app(argc, argv);

    QMainWindow window;
    auto *button = new QPushButton("Click Here!");
    window.setCentralWidget(button);
    window.resize(500, 500);
    window.show();

    QObject::connect(button, &QPushButton::clicked, [&] {
        QMessageBox::information(&window, "Hi!", "This is my first QT thingie ever, really");
    });

    return QApplication::exec();
}
