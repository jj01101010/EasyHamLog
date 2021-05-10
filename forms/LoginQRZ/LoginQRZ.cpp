#include "LoginQRZ.h"
#include "ui_LoginQRZ.h"

LoginQRZ::LoginQRZ(std::string username, bool firstTry, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginQRZ)
{
    ui->setupUi(this);
    ui->callsignEdit->setText(username.c_str());

    if (username != "") {
        ui->passwordEdit->setFocus();
    }

    if (!firstTry) {
        QLabel* tryAgain = new QLabel("<p style=\"color: red;\"><b>Wrong password or username. Try again.</b></p>");
        ui->verticalLayout_2->addWidget(tryAgain);
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
