#include "../../include/router/AppRouter.h"

AppRouter::AppRouter(QStackedWidget *stack, QObject *parent) : QObject(parent), stack_(stack) {
}

void AppRouter::registerPage(const Route r, QWidget *page) {
    pages_[r] = page;
    if (stack_->indexOf(page) < 0) stack_->addWidget(page);
}

void AppRouter::go(const Route r) {
    if (const auto it = pages_.find(r); it != pages_.end()) stack_->setCurrentWidget(it.value());
}
