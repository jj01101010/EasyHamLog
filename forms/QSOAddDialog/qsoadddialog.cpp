#include "qsoadddialog.h"
#include "ui_qsoadddialog.h"
#include <QMessageBox>
#include <CallsignLookup.h>
#include <QRZInterface.h>

EasyHamLog::QSOAddDialog::QSOAddDialog(EasyHamLog::MainUIApplication* parent, EasyHamLog::QSO* edited) :
    QDialog(parent),
    parent(parent),
    contest(false),
    ui(new Ui::QSOAddDialog)
{
    ui->setupUi(this);

    toggleContest(false);

    if (edited != nullptr) {

        // If there is a qso as a template set every form element

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

        if (edited->contest_number != "") {
            toggleContest();
            ui->contestNumberEdit->setText(edited->contest_number.c_str());
            ui->contestInfoEdit->setText(edited->contest_info.c_str());
        }

        setWindowTitle("Edit QSO");
        ui->title->setText("<p><span style=\" font-size:12pt; font-weight:600; \">Edit QSO</span></p>");

    }
    else {
        // If there is no template set the time and date to the current UTC time
        
        ui->deleteButton->hide();

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

    EasyHamLog::QSO* qso = parent->findQSOByCallsign(callsign.toStdString());

    if (qso == nullptr) {

        if (parent->preferences->useQRZ && callsign != "") {
            if (QRZInterface::getOpenQRZInterface()->HasLoggedIn()) {
                EasyHamLog::QSO* ham_member = QRZInterface::getOpenQRZInterface()->findQSOInQRZ(callsign.toStdString());

                if (ham_member != nullptr) {
                    ui->locatorEdit->setText(ham_member->locator.c_str());
                    ui->nameEdit->setText(ham_member->name.c_str());
                    ui->countryEdit->setText(ham_member->country.c_str());
                    delete ham_member;
                }
            }
        }

        if (ui->countryEdit->text() != "") {    // Already set the country
            ui->frequencyEdit->setFocus();  // Skip Prename edit, because it is already set
            return;
        }

        // get the prefix of the callsign by the last number in the callsign
        QString prefix;
        for (int i = callsign.length() - 1; i >= 0; i--) {
            if (callsign[i].isNumber()) {
                if (i < callsign.length() - 1) {
                    i++;    // Add the number itself
                }
                prefix = callsign.left(i);
                break;
            }
        }

        // We get the prefix object
        EasyHamLog::Callsign_Prefix* call_prefix = EasyHamLog::CallsignLookup::getPrefix(prefix);

        if (call_prefix == nullptr) {
            if (prefix.size() > 1) {
                call_prefix = EasyHamLog::CallsignLookup::getPrefix(prefix.left(prefix.size() - 1));
            }   
        }

        if (call_prefix == nullptr) {
            return;
        }

        // and set the country name
        ui->countryEdit->setText(call_prefix->country_name.c_str());
    }
    else {
        ui->nameEdit->setText(qso->name.c_str());
        ui->locatorEdit->setText(qso->locator.c_str());
        ui->countryEdit->setText(qso->country.c_str());
    }

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
    qso->contest_number = contest ? ui->contestNumberEdit->text().toStdString() : "";
    qso->contest_info = contest ? ui->contestInfoEdit->text().toStdString() : "";

    return qso;
}

void EasyHamLog::QSOAddDialog::on_deleteButton_clicked()
{
    this->done(QSO_ADD_DIALOG_RESULT_DELETE);
}

void EasyHamLog::QSOAddDialog::on_saveButton_clicked()
{
    this->done(QSO_ADD_DIALOG_RESULT_SAVE);
}

void EasyHamLog::QSOAddDialog::on_cancelButton_clicked()
{
    this->done(QSO_ADD_DIALOG_RESULT_CANCEL);
}

void EasyHamLog::QSOAddDialog::on_addContestInfoButton_clicked()
{
    toggleContest();
}

void EasyHamLog::QSOAddDialog::toggleContest(bool change_value)
{
    if (change_value) {
        contest = !contest;
    }

    ui->contestNumberEdit->setHidden(!contest);
    ui->contestInfoEdit->setHidden(!contest);
    ui->labelContestInfo->setHidden(!contest);
    ui->labelContestNumber->setHidden(!contest);
    ui->addContestInfoButton->setText(contest ? "-" : "+");   
}
