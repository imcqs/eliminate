#ifndef DIALOG_H
#define DIALOG_H

#include "mainwindow.h"
#include "aboutauthor.h"
#include <QtWidgets/QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_about_clicked(bool checked);

private:
    Ui::Dialog *ui;

    MainWindow *m_main;

    AboutAuthor *m_about;
};

#endif // DIALOG_H
