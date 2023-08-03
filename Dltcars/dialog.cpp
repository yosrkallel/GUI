#include "dialog.h"
#include "ui_dialog.h"
#include <QPixmap>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLTCars)
{
    ui->setupUi(this);
    QPixmap pix("C:/Users/bji/Downloads/gsquzh30lnq54zikbcs7.png");
    int x =ui->label_pic->width();
    int y =ui->label_pic->height();

    ui->label_pic->setPixmap(pix.scaled(x,y,Qt::KeepAspectRatio));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_log_in_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // Implement your login logic here.
    // For simplicity, let's assume the login is successful if the username and password are not empty.
    if (!username.isEmpty() && !password.isEmpty())
    {
        accept(); // Close the login dialog and return QDialog::Accepted
    }
}

