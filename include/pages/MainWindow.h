#ifndef LIBRARY_PROJECT_MAINWINDOW_H
#define LIBRARY_PROJECT_MAINWINDOW_H
#include <QMainWindow>

#include "../router/AppRouter.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stack_ = nullptr;
    AppRouter *router_;
};

#endif
