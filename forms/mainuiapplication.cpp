#include "mainuiapplication.h"
#include "ui_mainuiapplication.h"
#include <QSODatabaseInterface.h>
#include <qsoadddialog.h>
#include <QDir>
#include <qsomap.h>
#include <prefixlookupdialog.h>
#include <CallsignLookup.h>
#include <ADIInterface.h>

#define MAIN_SORT_ORDER Qt::DescendingOrder     // Sorting order of the QSOs (date and time)

// Constants for the databases and prefix lists
#define QSO_DATABASE_DIR "saved/qso_database.xml"

EasyHamLog::MainUIApplication::MainUIApplication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUIApplication)
{
    ui->setupUi(this);
    // Hide UUID column
    ui->tableWidget->hideColumn(ui->tableWidget->columnCount() - 1);

    // Create the "saved" and "lookups" folder
    // If they don't already exist

    QDir root_dir;
    qInfo(root_dir.absolutePath().toStdString().c_str());

    if (!root_dir.mkpath("./saved")) {
        QMessageBox::warning(this, "Folder Error!", "Could not create folder 'saved'");
        setupSuccess = false;
        return;
    }

    if (!root_dir.mkpath("./lookups")) {
        QMessageBox::warning(this, "Folder Error!", "Could not create folder 'lookups'");
        setupSuccess = false;
        return;
    }

    // Read the local QSO database
    QSO_DATABASE_ELEMENT root;
    QSO_DATABASE* database = nullptr;

    if (!EasyHamLog::QSODatabaseInterface::readDatabase(QSO_DATABASE_DIR, database, &root, true)) {
        setupSuccess = false;
        return;
    }
    
    // Register every QSO into the vector and the table
    EasyHamLog::QSO* current_qso;
    while ((current_qso = EasyHamLog::QSODatabaseInterface::nextQSO(&root)) != nullptr) {
        registeredQSOs.insert(registeredQSOs.end(), current_qso);
        insertRowData(ui->tableWidget, 0, current_qso);
    }

    delete database;

    // Sort the table after Date and Time
    ui->tableWidget->sortItems(2, MAIN_SORT_ORDER);
    ui->tableWidget->sortItems(3, MAIN_SORT_ORDER);

    EasyHamLog::CallsignLookup::Initialize();
}

EasyHamLog::MainUIApplication::~MainUIApplication()
{
    // Delete every qso and prefix, which was created on the heap
    for (auto& qso : registeredQSOs) {
        delete qso;
    }
    delete ui;

    EasyHamLog::CallsignLookup::Destroy();
}

EasyHamLog::QSO* EasyHamLog::MainUIApplication::findQSOByCallsign(const std::string& callsign) const
{
    for (auto& qso : registeredQSOs) {
        if (qso->callsign == callsign) {
            return qso;
        }
    }
    return nullptr;
}

void EasyHamLog::MainUIApplication::on_addContactButton_clicked()
{
    // Create a new Dialog
    EasyHamLog::QSOAddDialog addDialog(this);
    addDialog.setModal(true);
    int ret = addDialog.exec();     // 1 = Ok, 0 = Cancel

    if (ret == QSO_ADD_DIALOG_RESULT_SAVE) { // If the Save button was clicked

        // We get the QSO information
        EasyHamLog::QSO* qso = addDialog.getQSO();

        // add the qso to the vector
        registeredQSOs.push_back(qso);

        // Write the new qsos to the database
        EasyHamLog::QSODatabaseInterface::writeDatabase(QSO_DATABASE_DIR, registeredQSOs);

        // insert it into the table
        insertRowData(ui->tableWidget, 0, qso);

        // and resort the table
        ui->tableWidget->sortItems(2, MAIN_SORT_ORDER);
        ui->tableWidget->sortItems(3, MAIN_SORT_ORDER);

    }
}

void EasyHamLog::MainUIApplication::on_findContactEdit_textEdited(const QString &arg1)
{
    // Find every QSO with some equal match
    QList<QTableWidgetItem*> tempTable = ui->tableWidget->findItems(arg1, Qt::MatchContains);
    
    // And hide every row, which is not in the tempTable
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHidden(i, !tempTable.contains(ui->tableWidget->item(i, 0)));
    }
}

void EasyHamLog::MainUIApplication::on_actionQuit_triggered()
{
    // Quit the app
    QApplication::quit();
}

void EasyHamLog::MainUIApplication::setRowData(QTableWidget* table, int row, EasyHamLog::QSO* qso)
{
    // Set every row in the table
    EasyHamLog::QSO _qso = *qso;
    table->setItem(row, 0, new QTableWidgetItem(_qso.callsign.c_str()));
    table->setItem(row, 1, new QTableWidgetItem(_qso.name.c_str()));
    table->setItem(row, 2, new QTableWidgetItem(_qso.time.c_str()));
    table->setItem(row, 3, new QTableWidgetItem(_qso.date.c_str()));
    table->setItem(row, 4, new QTableWidgetItem(_qso.opmode.c_str()));
    table->setItem(row, 5, new QTableWidgetItem(_qso.band.c_str()));
    table->setItem(row, 6, new QTableWidgetItem(_qso.freq.c_str()));
    table->setItem(row, 7, new QTableWidgetItem(_qso.rst.c_str()));
    table->setItem(row, 8, new QTableWidgetItem(_qso.locator.c_str()));
    table->setItem(row, 9, new QTableWidgetItem(_qso.country.c_str()));
}

void EasyHamLog::MainUIApplication::insertRowData(QTableWidget* table, int row, EasyHamLog::QSO* qso) {
    // Create a new row
    table->insertRow(row);
    
    // Create a new uuid and correspond the uuid with the current qso
    QString uuid = QUuid::createUuid().toString();
    qsoRows[uuid.toStdString()] = qso;

    // Set the row data
    setRowData(table, row, qso);

    // Set the UUID in the table column
    table->setItem(row, table->columnCount() - 1, new QTableWidgetItem(uuid));
}


void EasyHamLog::MainUIApplication::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    // Find double clicked qso by getting the uuid of the row
    EasyHamLog::QSO* qso = qsoRows[ui->tableWidget->item(item->row(), ui->tableWidget->columnCount() - 1)->text().toStdString()];
    // Make a new dialog with the qso as a template
    EasyHamLog::QSOAddDialog addDialog(this, qso);
    addDialog.setModal(true);
    int ret = addDialog.exec();


    if (ret == QSO_ADD_DIALOG_RESULT_SAVE) {  // If we want to save the qso
        
        // Get QSO indices
        QString uuid;
        for (std::pair<std::string, EasyHamLog::QSO*> qsos : qsoRows) {
            if (qsos.second == qso) {
                uuid = qsos.first.c_str();
                break;
            }
        }

        int qso_index = std::distance(registeredQSOs.begin(), std::find(registeredQSOs.begin(), registeredQSOs.end(), qso));

        // Remove qso from registered QSO list
        delete registeredQSOs[qso_index];
        registeredQSOs.erase(registeredQSOs.begin() + qso_index);

        qso = addDialog.getQSO();

        // Re-add the qso
        registeredQSOs.push_back(qso);
        qsoRows[uuid.toStdString()] = qso;

        QList<QTableWidgetItem*> items = ui->tableWidget->findItems(uuid, Qt::MatchExactly);

        int rowIndex = items[0]->row();

        // Set the new row data
        setRowData(ui->tableWidget, rowIndex, qso);

        // And save the new database
        EasyHamLog::QSODatabaseInterface::writeDatabase(QSO_DATABASE_DIR, registeredQSOs);

    }
    else if(ret == QSO_ADD_DIALOG_RESULT_DELETE) {
        // Get QSO indices
        QString uuid;
        for (std::pair<std::string, EasyHamLog::QSO*> qsos : qsoRows) {
            if (qsos.second == qso) {
                uuid = QString(qsos.first.c_str());
                break;
            }
        }

        int qso_index = std::distance(registeredQSOs.begin(), std::find(registeredQSOs.begin(), registeredQSOs.end(), qso));

        QList<QTableWidgetItem*> items = ui->tableWidget->findItems(uuid, Qt::MatchExactly);

        // Remove the QSO row
        ui->tableWidget->removeRow(items[0]->row());

        // Remove qso from registered QSO list
        delete registeredQSOs[qso_index];
        registeredQSOs.erase(registeredQSOs.begin() + qso_index);

        qsoRows.erase(uuid.toStdString());

        // And save the new database
        EasyHamLog::QSODatabaseInterface::writeDatabase(QSO_DATABASE_DIR, registeredQSOs);
    }
}

void EasyHamLog::MainUIApplication::on_actionPrefix_Lookup_triggered()
{
    EasyHamLog::PrefixLookupDialog dialog(this);
    dialog.exec();
}

void EasyHamLog::MainUIApplication::on_actionExport_Database_triggered()
{
    QDir root_dir;

    if (!root_dir.mkpath("exports")) {
        QMessageBox::warning(this, "Folder Error!", "Could not create folder 'saved'");
    }
    EasyHamLog::ADIInterface::writeADIFile("exports/export.adi", registeredQSOs);
}
