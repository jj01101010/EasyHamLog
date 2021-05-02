#include "AboutMeDialog.h"
#include "ui_AboutMeDialog.h"

AboutMeDialog::AboutMeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMeDialog)
{
    ui->setupUi(this);
}

AboutMeDialog::~AboutMeDialog()
{
    delete ui;
}
