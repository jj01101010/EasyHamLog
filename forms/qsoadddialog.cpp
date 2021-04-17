#include "qsoadddialog.h"
#include "ui_qsoadddialog.h"
#include <QMessageBox>

QSOAddDialog::QSOAddDialog(MainUIApplication* parent) :
    QDialog(parent),
    parent(parent),
    ui(new Ui::QSOAddDialog)
{
    ui->setupUi(this);

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
}

QSOAddDialog::~QSOAddDialog()
{
    delete ui;
}

void QSOAddDialog::on_fnButton_clicked()
{
    ui->rapportEdit->setText("59");
}

void QSOAddDialog::on_fnnButton_clicked()
{
    ui->rapportEdit->setText("599");
}

void QSOAddDialog::on_callsignEdit_editingFinished() {
    QString callsign = ui->callsignEdit->text();


    QString prefix;
    for (int i = callsign.length() - 1; i >= 0; i--) {
        if (callsign[i].isNumber()) {
            prefix = callsign.left(i);
            break;
        }
    }

    HamLog::Callsign_Prefix* call_prefix = parent->getPrefix(prefix);

    if (call_prefix == nullptr) {
        return;
    }

    ui->countryEdit->setText(call_prefix->country_name.c_str());

}


HamLog::QSO* QSOAddDialog::getQSO() const {
    HamLog::QSO* qso = new HamLog::QSO;
    qso->callsign = ui->callsignEdit->text().toStdString();
    qso->name = ui->nameEdit->text().toStdString();
    qso->time = ui->dateTimeEdit->time().toString().toStdString();
    qso->date = ui->dateTimeEdit->date().toString().toStdString();
    qso->band = ui->bandComboBox->currentText().toStdString();
    qso->country = ui->countryEdit->text().toStdString();
    qso->freq = ui->frequencyEdit->text().toStdString();
    qso->locator = ui->locatorEdit->text().toStdString();
    qso->opmode = ui->opmodeEdit->text().toStdString();
    qso->rst = ui->rapportEdit->text().toStdString();

    return qso;
}


