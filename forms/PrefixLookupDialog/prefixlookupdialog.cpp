#include "prefixlookupdialog.h"
#include "ui_prefixlookupdialog.h"
#include <MainUIApplication/mainuiapplication.h>
#include <CallsignLookup.h>

EasyHamLog::PrefixLookupDialog::PrefixLookupDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PrefixLookupDialog)
{
    ui->setupUi(this);
}

EasyHamLog::PrefixLookupDialog::~PrefixLookupDialog()
{
    delete ui;
}

void EasyHamLog::PrefixLookupDialog::on_lineEdit_textEdited(const QString &line)
{
    EasyHamLog::Callsign_Prefix* prefix = EasyHamLog::CallsignLookup::getPrefix(line);
    if (prefix != nullptr) {
        ui->countryLabel->setText(prefix->country_name.c_str());
    }
    else {
        ui->countryLabel->setText("");
    }
}
