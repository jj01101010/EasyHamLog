#include "mainuiapplication.h"
#include "ui_mainuiapplication.h"
#include <QSODatabaseInterface.h>
#include <qsoadddialog.h>
#include <QDir>

#define MAIN_SORT_ORDER Qt::DescendingOrder     // Sorting order of the QSOs (date and time)

// Constants for the databases and prefix lists
#define QSO_DATABASE_DIR "saved/qso_database.xml"
#define PREFIX_DATABASE_DIR "lookups/prefix_lookup.xml"

EasyHamLog::MainUIApplication::MainUIApplication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUIApplication)
{
    ui->setupUi(this);
    // Hide UUID column
    ui->tableWidget->hideColumn(ui->tableWidget->columnCount() - 1);

    // Create the "saved" and "lookups" folder
    // If they don't already exist
    QDir temp_dir("saved");
    if (!temp_dir.exists()) {
        if (!temp_dir.mkdir("../saved")) {
            QMessageBox::warning(this, "Folder Error!", "Could not create folder 'saved'");
        }
    }

    temp_dir.setPath("lookups");
    if (!temp_dir.exists()) {
        if (!temp_dir.mkdir("../lookups")) {
            QMessageBox::warning(this, "Folder Error!", "Could not create folder 'lookups'");
        }
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

    database = nullptr;

    // Read the Prefix list and store it in the unsorted_list
    if (!EasyHamLog::QSODatabaseInterface::readDatabase(PREFIX_DATABASE_DIR, database, &root)) {
        setupSuccess = false;
        return;
    }

    QSO_DATABASE_ELEMENT current_prefix;

    while (!(current_prefix = EasyHamLog::QSODatabaseInterface::nextElement(&root)).isNull()) {
        EasyHamLog::Callsign_Prefix* prefix = new EasyHamLog::Callsign_Prefix;
        prefix->country_name = current_prefix.attribute("name", "").toStdString();
        prefix->prefix = current_prefix.attribute("prefix", "").toStdString();
        prefix->cq_region = current_prefix.attribute("cq", "").toInt();
        prefix->itu_region = current_prefix.attribute("itu", "").toInt();

        callsignPrefixes.push_back(prefix);
    }

    // Sort the table after Date and Time
    ui->tableWidget->sortItems(2, MAIN_SORT_ORDER);
    ui->tableWidget->sortItems(3, MAIN_SORT_ORDER);

    delete database;

}

EasyHamLog::MainUIApplication::~MainUIApplication()
{
    // Delete every qso and prefix, which was created on the heap
    for (auto& qso : registeredQSOs) {
        delete qso;
    }
    for (auto& prefix : callsignPrefixes) {
        delete prefix;
    }
    delete ui;
}

// Helper function to split a string by one seperator
std::vector<std::string> EasyHamLog::MainUIApplication::splitString(const char seperator, const std::string& s) {
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);
        
        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

// Get a prefix object with the country name by looking up the callsign prefix in the lookup table
EasyHamLog::Callsign_Prefix* EasyHamLog::MainUIApplication::getPrefix(const QString& call_prefix) {
    for (auto& prefix : callsignPrefixes) {
        std::vector<std::string> splitted = splitString('-', prefix->prefix);   // Split the current callsign prefix by - (from xa to xz is xa-xz)

        // If there is only splitted object it means there is only one prefix for a country (e.g. 4X for israel)
        if (splitted.size() == 1) {
            if (splitted[0] == std::string(call_prefix.toStdString())) {
                return prefix;
            }
        }
        // If there is a range of callsign prefixes we want to see if the first letter matches in both the callsign prefix and also the current prefix
        // Also we need to check if the second letter of the callsign prefix is in the range of the current prefix
        else {
            if (splitted[0][1] <= call_prefix[1] && splitted[1][1] >= call_prefix[1] && splitted[0][0] == call_prefix[0]) {
                return prefix;
            }
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
    
    if (ret == 1) { // If the Ok button was clicked

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
    qsoRows[uuid] = qso;

    // Set the row data
    setRowData(table, row, qso);

    // Set the UUID in the table column
    table->setItem(row, table->columnCount() - 1, new QTableWidgetItem(uuid));
}


void EasyHamLog::MainUIApplication::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    // Find double clicked qso by getting the uuid of the row
    EasyHamLog::QSO* qso = qsoRows[ui->tableWidget->item(item->row(), ui->tableWidget->columnCount() - 1)->text()];
    // Make a new dialog with the qso as a template
    EasyHamLog::QSOAddDialog addDialog(this, qso);
    addDialog.setModal(true);
    int ret = addDialog.exec();


    if (ret) {  // If we want to save the qso
        
        // Get QSO indices
        QString uuid;
        for (std::pair<QString, EasyHamLog::QSO*> qsos : qsoRows) {
            if (qsos.second == qso) {
                uuid = qsos.first;
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
        qsoRows[uuid] = qso;

        QList<QTableWidgetItem*> items = ui->tableWidget->findItems(uuid, Qt::MatchExactly);

        int rowIndex = items[0]->row();

        // Set the new row data
        setRowData(ui->tableWidget, rowIndex, qso);

        // And save the new database
        EasyHamLog::QSODatabaseInterface::writeDatabase(QSO_DATABASE_DIR, registeredQSOs);

    }
}
