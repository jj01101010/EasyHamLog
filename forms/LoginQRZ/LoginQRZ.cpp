#include "LoginQRZ.h"
#include "ui_LoginQRZ.h"

LoginQRZ::LoginQRZ(std::string username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginQRZ)
{
    ui->setupUi(this);
    ui->callsignEdit->setText(username.c_str());

    if (username != "") {
        ui->passwordEdit->setFocus();
    }
}

LoginQRZ::~LoginQRZ()
{
    delete ui;
}

void LoginQRZ::getLoginCredentials(std::string* username, std::string* password) const
{
    *username = ui->callsignEdit->text().toStdString();
    *password = ui->passwordEdit->text().toStdString();
}
