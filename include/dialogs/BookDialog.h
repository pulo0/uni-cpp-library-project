#ifndef LIBRARY_PROJECT_BOOKDIALOG_H
#define LIBRARY_PROJECT_BOOKDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>

#include "../structs/Book.h"

class BookDialog : public QDialog {
    Q_OBJECT;

public:
    explicit BookDialog(QWidget *parent = nullptr);

    explicit BookDialog(const Book &book, QWidget *parent = nullptr);

    std::string title() const;
    std::string author() const;
    std::string description() const;
    int year_of_publish() const;
    CoverType cover() const;

private slots:
    void validate();

private:
    void setup_ui();
    void prefill(const Book &book);

    QLineEdit *title_ = nullptr;
    QLineEdit *author_ = nullptr;
    QTextEdit *description_ = nullptr;
    QSpinBox *year_of_publish_ = nullptr;
    QComboBox *cover_ = nullptr;
    QDialogButtonBox *buttons_ = nullptr;
};

#endif
