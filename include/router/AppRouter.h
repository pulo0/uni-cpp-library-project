#ifndef LIBRARY_PROJECT_APPROUTER_H
#define LIBRARY_PROJECT_APPROUTER_H
#include <QStackedWidget>

enum class Route {
    LoginPage,
    UserMain, UserOptions, UserRented, UserAvailable,
    AdminPage,
};

class AppRouter : public QObject {
    Q_OBJECT;

public:
    explicit AppRouter(QStackedWidget *stack, QObject *parent = nullptr);

    void registerPage(Route r, QWidget *page);

public slots:
    void go(Route r);

private:
    QStackedWidget *stack_ = nullptr;
    QHash<Route, QWidget *> pages_;
};

#endif
