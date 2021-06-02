#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

EasyHamLog::PreferencesDialog::PreferencesDialog(EasyHamLog::Preferences* preferences, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    ui->callsignEdit->setText(preferences->callsign.c_str());
    ui->locatorEdit->setText(preferences->locator.c_str());
    ui->useQRZCheckBox->setChecked(preferences->useQRZ);
}

EasyHamLog::PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

EasyHamLog::Preferences* EasyHamLog::PreferencesDialog::getPreferences() const
{
    Preferences* preferences = new Preferences;
    preferences->useQRZ = ui->useQRZCheckBox->isChecked();
    preferences->callsign = ui->callsignEdit->text().toStdString();
    preferences->locator = ui->locatorEdit->text().toStdString();
    return preferences;
}
