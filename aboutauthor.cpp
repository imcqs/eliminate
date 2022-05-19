#include "aboutauthor.h"
#include "ui_aboutauthor.h"

AboutAuthor::AboutAuthor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutAuthor)
{
    ui->setupUi(this);
    setWindowTitle("关于作者");
    setFixedSize(480, 270);
}

AboutAuthor::~AboutAuthor()
{
    delete ui;
}

void AboutAuthor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/images/back.jpg"));

    painter.setPen(QColor(255, 0, 0));
    painter.setFont(QFont("", 30));
    painter.drawText(160, 110, "拒绝游戏");
    painter.drawText(160, 160, "从我做起");
}
