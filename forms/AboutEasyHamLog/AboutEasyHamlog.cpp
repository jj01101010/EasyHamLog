#include "AboutEasyHamlog.h"
#include "ui_AboutEasyHamlog.h"
#include <QMessageBox>

AboutEasyHamLog::AboutEasyHamLog(QString version, int app_version_encoded, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutEasyHamLog)
{
    ui->setupUi(this);

    
    ui->title->setText("About EasyHamLog v" + version + " 0x" + QString("%1").arg(app_version_encoded, 1, 16).toUpper());
}

AboutEasyHamLog::~AboutEasyHamLog()
{
    delete ui;
}
