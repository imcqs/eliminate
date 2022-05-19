#ifndef ABOUTAUTHOR_H
#define ABOUTAUTHOR_H

#include <QtWidgets/QWidget>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

namespace Ui {
class AboutAuthor;
}

class AboutAuthor : public QWidget
{
    Q_OBJECT

public:
    explicit AboutAuthor(QWidget *parent = nullptr);
    ~AboutAuthor();

    void paintEvent(QPaintEvent*);

private:
    Ui::AboutAuthor *ui;
};

#endif // ABOUTAUTHOR_H
