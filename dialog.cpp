#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("打字v2.0");
    ui->label->setFont(QFont("", 30));
    ui->label->setText("适度游戏");
    ui->label2->setFont(QFont("", 20));
    ui->label2->setText("自行选择游戏难度");
    setFixedSize(400, 300);

    m_about = new AboutAuthor();

    connect(ui->pushButton_start, &QPushButton::clicked, [=](){
        this->hide();
        m_main = new MainWindow;
        m_main->show();

        connect(m_main, &MainWindow::toMenu, [=](){
            m_main->close();
            this->show();
        });
    });
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_about_clicked(bool checked)
{
    Q_UNUSED(checked)
    m_about->show();
}
