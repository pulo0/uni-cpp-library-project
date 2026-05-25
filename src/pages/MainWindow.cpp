#include "../../include/pages/MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);

    router_ = new AppRouter(stack_, this);

}
