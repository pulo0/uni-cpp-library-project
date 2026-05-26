#include "../../include/dialogs/BookDialog.h"
#include <QFormLayout>
#include <QPushButton>

BookDialog::BookDialog(QWidget *parent) : QDialog(parent) {
    setup_ui();
    validate();
}

BookDialog::BookDialog(const Book &book, QWidget *parent) : QDialog(parent) {
    setup_ui();
    prefill(book);
    validate();
}

void BookDialog::setup_ui() {
    setWindowTitle("Book");

    title_ = new QLineEdit(this);
    author_ = new QLineEdit(this);
    description_ = new QTextEdit(this);

    year_of_publish_ = new QSpinBox(this);
    year_of_publish_->setRange(0, 2100);
    year_of_publish_->setValue(2000);

    cover_ = new QComboBox(this);
    cover_->addItem("Paperback", static_cast<int>(CoverType::Paperback));
    cover_->addItem("Hardcover", static_cast<int>(CoverType::Hardcover));

    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *form = new QFormLayout;
    form->addRow("Title:", title_);
    form->addRow("Author:", author_);
    form->addRow("Description:", description_);
    form->addRow("Year of Publish:", year_of_publish_);
    form->addRow("Cover:", cover_);

    auto *root = new QVBoxLayout(this);
    root->addLayout(form);
    root->addWidget(buttons_);

    connect(buttons_, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons_, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(title_, &QLineEdit::textChanged, this, &BookDialog::validate);
    connect(author_, &QLineEdit::textChanged, this, &BookDialog::validate);
}

void BookDialog::prefill(const Book &book) {
    title_->setText(QString::fromStdString(book.title));
    author_->setText(QString::fromStdString(book.author));
    description_->setPlainText(QString::fromStdString(book.description));
    year_of_publish_->setValue(book.year_of_publish);
    if (const int idx = cover_->findData(static_cast<int>(book.cover)); idx >= 0) cover_->setCurrentIndex(idx);
}

void BookDialog::validate() {
    const bool ok = !title_->text().trimmed().isEmpty() && !author_->text().trimmed().isEmpty();

    if (auto *ok_btn = buttons_->button(QDialogButtonBox::Ok)) ok_btn->setEnabled(ok);
}

std::string BookDialog::title() const {
    return title_->text().trimmed().toStdString();
}

std::string BookDialog::author() const {
    return author_->text().trimmed().toStdString();
}

std::string BookDialog::description() const {
    return description_->toPlainText().trimmed().toStdString();
}

int BookDialog::year_of_publish() const {
    return year_of_publish_->value();
}

CoverType BookDialog::cover() const {
    return static_cast<CoverType>(cover_->currentData().toInt());
}
