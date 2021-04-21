#include "qsoadddialog.h"
#include "ui_qsoadddialog.h"
#include <QMessageBox>

EasyHamLog::QSOAddDialog::QSOAddDialog(EasyHamLog::MainUIApplication* parent, EasyHamLog::QSO* edited) :
    QDialog(parent),
    parent(parent),
    ui(new Ui::QSOAddDialog)
{
    ui->setupUi(this);

    // If there is a qso as a template set every form
    if (edited != nullptr) {
        ui->callsignEdit->setText(edited->callsign.c_str());
        ui->nameEdit->setText(edited->name.c_str());
        ui->frequencyEdit->setText(edited->freq.c_str());
        ui->rapportEdit->setText(edited->rst.c_str());
        ui->opmodeEdit->setText(edited->opmode.c_str());
        ui->bandComboBox->setCurrentIndex(ui->bandComboBox->findText(edited->band.c_str()));
        ui->dateTimeEdit->setTime(QTime::fromString(edited->time.c_str()));
        ui->dateTimeEdit->setDate(QDate::fromString(edited->date.c_str(), "dd.MM.yyyy ddd"));
        ui->locatorEdit->setText(edited->locator.c_str());
        ui->countryEdit->setText(edited->country.c_str());
    }
    else {
        // If there is no template set the time and date to the current UTC time
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
    }
}

EasyHamLog::QSOAddDialog::~QSOAddDialog()
{
    delete ui;
}

void EasyHamLog::QSOAddDialog::on_fnButton_clicked()
{
    ui->rapportEdit->setText("59"); // Set the rapport to 59
}

void EasyHamLog::QSOAddDialog::on_fnnButton_clicked()
{
    ui->rapportEdit->setText("599"); // Set the rapport to 599
}

void EasyHamLog::QSOAddDialog::on_callsignEdit_editingFinished() {
    // If we finished typing the callsign we get the text
    QString callsign = ui->callsignEdit->text();

    // get the prefix of the callsign by the last number in the callsign
    QString prefix;
    for (int i = callsign.length() - 1; i >= 0; i--) {
        if (callsign[i].isNumber()) {
            prefix = callsign.left(i);
            break;
        }
    }

    // We get the prefix object
    EasyHamLog::Callsign_Prefix* call_prefix = parent->getPrefix(prefix);

    if (call_prefix == nullptr) {
        return;
    }

    // and set the country name
    ui->countryEdit->setText(call_prefix->country_name.c_str());

}


EasyHamLog::QSO* EasyHamLog::QSOAddDialog::getQSO() const {
    // Return information about the qso
    EasyHamLog::QSO* qso = new EasyHamLog::QSO;
    qso->callsign = ui->callsignEdit->text().toStdString();
    qso->name = ui->nameEdit->text().toStdString();
    qso->time = ui->dateTimeEdit->time().toString().toStdString();
    qso->date = ui->dateTimeEdit->date().toString("dd.MM.yyyy ddd").toStdString();
    qso->band = ui->bandComboBox->currentText().toStdString();
    qso->country = ui->countryEdit->text().toStdString();
    qso->freq = ui->frequencyEdit->text().toStdString();
    qso->locator = ui->locatorEdit->text().toStdString();
    qso->opmode = ui->opmodeEdit->text().toStdString();
    qso->rst = ui->rapportEdit->text().toStdString();

    return qso;
}


