#include "mainuiapplication.h"
#include "ui_mainuiapplication.h"
#include <QSODatabaseInterface.h>

#define MAIN_SORT_ORDER Qt::DescendingOrder

MainUIApplication::MainUIApplication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUIApplication)
{
    ui->setupUi(this);
    
    QSO_DATABASE* qso_database = nullptr;
    QSO_DATABASE_ELEMENT root;

    if (!HamLog::QSODatabaseInterface::readDatabase("database.xml", qso_database, &root)) {
        setupSuccess = false;
        return;
    }
    
    HamLog::QSO* current_qso;
    while ((current_qso = HamLog::QSODatabaseInterface::nextQSO(&root)) != nullptr) {
        registeredQSOs.insert(registeredQSOs.end(), current_qso);
    }

    delete qso_database;

    for (auto& qso : registeredQSOs) {
        insertRowData(ui->tableWidget, ui->tableWidget->rowCount(), qso);
    }

    ui->tableWidget->sortItems(0, MAIN_SORT_ORDER);
}

MainUIApplication::~MainUIApplication()
{
    for (auto& qso : registeredQSOs) {
        delete qso;
    }
    delete ui;
}

void MainUIApplication::on_addContactButton_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->sortItems(0, MAIN_SORT_ORDER);
}

void MainUIApplication::on_findContactEdit_textEdited(const QString &arg1)
{
    QList<QTableWidgetItem*> tempTable = ui->tableWidget->findItems(arg1, Qt::MatchContains);
    
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHidden(i, !tempTable.contains(ui->tableWidget->item(i, 0)));
    }

}

void MainUIApplication::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainUIApplication::insertRowData(QTableWidget* table, int row, HamLog::QSO* qso) {
    HamLog::QSO _qso = *qso;
    table->insertRow(row);
    table->setItem(row, 0, new QTableWidgetItem(_qso.callsign.c_str()));
    table->setItem(row, 1, new QTableWidgetItem(_qso.name.c_str()));
    table->setItem(row, 2, new QTableWidgetItem(_qso.time.c_str()));
    table->setItem(row, 3, new QTableWidgetItem(_qso.date.c_str()));
    table->setItem(row, 4, new QTableWidgetItem(_qso.band.c_str()));
    table->setItem(row, 5, new QTableWidgetItem(_qso.freq.c_str()));
    table->setItem(row, 6, new QTableWidgetItem(_qso.locator.c_str()));
    table->setItem(row, 7, new QTableWidgetItem(_qso.country.c_str()));
}

