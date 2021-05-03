#include "mainuiapplication.h"
#include "ui_mainuiapplication.h"
#include <QSODatabaseInterface.h>
#include <QSOAddDialog/qsoadddialog.h>
#include <QDir>
#include <QSOMap/qsomap.h>
#include <PrefixLookupDialog/prefixlookupdialog.h>
#include <CallsignLookup.h>
#include <ADIInterface.h>
#include <QCheckBox>
#include <QFileDialog>
#include <QLabel>
#include <QBoxLayout>
#include <AboutEasyHamLog/AboutEasyHamlog.h>

#define MAIN_SORT_ORDER Qt::DescendingOrder     // Sorting order of the QSOs (date and time)

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

    newSession();

    EasyHamLog::CallsignLookup::Initialize();

    app_version = EHL_APP_VERSION;

    // Convert Version to major, minor, patch
    QString curr_int = "";
    unsigned char major = 0, minor = 0, patch = 0;
    for (int i = 0, dot_counter = 0; i < app_version.size() + 1; i++) {
        if (app_version[i] == '.' || i == app_version.size()) {
            switch (dot_counter)
            {
            case 0:
                major = (unsigned char)curr_int.toInt();
                break;
            case 1:
                minor = (unsigned char)curr_int.toInt();
                break;
            case 2:
                patch = (unsigned char)curr_int.toInt();
                break;
            default:
                QMessageBox::critical(this, "Version error", "Could not decode version. To my seperators!", QMessageBox::Ok);
                break;
            }

            curr_int = "";
            dot_counter++;
            continue;
        }
        if (i >= app_version.size()) {
            break;
        }
        curr_int += app_version[i];
    }

    // Create a hex coded version
    app_version_encoded = (major << 8) ^ (minor << 4) ^ patch;

    this->setWindowTitle("EasyHamLog - v" + app_version + " - by Jannis Leon Jung - DO9JJ");

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
        EasyHamLog::QSODatabaseInterface::writeDatabase(databasePath, registeredQSOs);

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
    table->setCellWidget(row, 10, new QCheckBox());
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

void EasyHamLog::MainUIApplication::newSession()
{
    databasePath = QString("saved/session-%1.xml").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy--hh-mm-ss"));
    qsoRows.clear();
    registeredQSOs.clear();

    while (ui->tableWidget->rowCount() > 0) {
        ui->tableWidget->removeRow(0);
    }
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
        EasyHamLog::QSODatabaseInterface::writeDatabase(databasePath, registeredQSOs);

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
        EasyHamLog::QSODatabaseInterface::writeDatabase(databasePath, registeredQSOs);
    }
}

void EasyHamLog::MainUIApplication::on_actionPrefix_Lookup_triggered()
{
    EasyHamLog::PrefixLookupDialog dialog(this);
    dialog.exec();
}

void EasyHamLog::MainUIApplication::on_actionExport_Database_triggered()
{

    std::vector<EasyHamLog::QSO*> toExport;

    for (size_t i = 0; i < ui->tableWidget->rowCount(); i++) {
        QCheckBox* box = (QCheckBox*)ui->tableWidget->cellWidget(i, 10);

        if (box->isChecked()) {
            std::string uuid = ui->tableWidget->item(i, 11)->text().toStdString();

            for (auto& pair : qsoRows) {
                if (pair.first == uuid) {
                    toExport.push_back(pair.second);
                    break;
                }
            }
        }
    }

    QDir root_dir;
    if (!root_dir.mkpath("exports")) {
        QMessageBox::warning(this, "Folder Error!", "Could not create folder 'saved'");
    }

    QString name = QString("exports/export-%1.adi").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy--hh-mm-ss"));

    qInfo(name.toStdString().c_str());

    if (toExport.size() == 0) {
        int ret = QMessageBox::information(this, "Export Status", "Do you want to export the complete database?", QMessageBox::Yes, QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            EasyHamLog::ADIInterface::writeADIFile(name, registeredQSOs);
        }
    }else{
        EasyHamLog::ADIInterface::writeADIFile(name, toExport);
    }

}

void EasyHamLog::MainUIApplication::on_actionNew_Session_triggered()
{
    newSession();
}

void EasyHamLog::MainUIApplication::on_actionOpen_Session_triggered()
{

    QFileDialog dialog(this, "Select Database", QApplication::applicationDirPath());

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("XML-Files (*.xml)");

    int ret = dialog.exec();

    if (ret == 0) {
        return;
    }

    newSession();

    databasePath = dialog.selectedFiles()[0];
    
    // Read the local QSO database
    QSO_DATABASE_ELEMENT root;
    QSO_DATABASE* database = nullptr;

    if (!EasyHamLog::QSODatabaseInterface::readDatabase(databasePath, database, &root, true)) {
        //setupSuccess = false;
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
}

void EasyHamLog::MainUIApplication::on_actionAbout_me_triggered()
{
    AboutEasyHamLog dialog(app_version, app_version_encoded, this);  // Version by CMake
    dialog.setModal(true);
    dialog.exec();
}
