#include "../../include/router/AppRouter.h"

AppRouter::AppRouter(QStackedWidget *stack, QObject *parent) : QObject(parent), stack_(stack) {
}

/// Addition of the pages into working window of the application (simply all available pages to jump into).
///
/// Function registers a page into a hash table and then if it not exists on the stack, adds a page's widget to it
void AppRouter::registerPage(const Route r, QWidget *page) {
    pages_[r] = page;
    if (stack_->indexOf(page) < 0) stack_->addWidget(page);
}

/// Finds registered page from the hash table, if exists --> proceed to change currently viewed page in the stack
void AppRouter::go(const Route r) {
    if (const auto it = pages_.find(r); it != pages_.end()) stack_->setCurrentWidget(it.value());
}
