#include "mainuiapplication.h"
#include "ui_mainuiapplication.h"
#include <QSODatabaseInterface.h>
#include <qsoadddialog.h>

#define MAIN_SORT_ORDER Qt::DescendingOrder

EasyHamLog::MainUIApplication::MainUIApplication(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUIApplication)
{
    ui->setupUi(this);
    ui->tableWidget->hideColumn(ui->tableWidget->columnCount() - 1);

    QSO_DATABASE_ELEMENT root;
    QSO_DATABASE* database = nullptr;

    if (!EasyHamLog::QSODatabaseInterface::readDatabase("database.xml", database, &root, true)) {
        setupSuccess = false;
        return;
    }
    
    EasyHamLog::QSO* current_qso;
    while ((current_qso = EasyHamLog::QSODatabaseInterface::nextQSO(&root)) != nullptr) {
        registeredQSOs.insert(registeredQSOs.end(), current_qso);
    }

    delete database;

    for (auto& qso : registeredQSOs) {
        insertRowData(ui->tableWidget, 0, qso);
    }

    ui->tableWidget->sortItems(2, MAIN_SORT_ORDER);
    ui->tableWidget->sortItems(3, MAIN_SORT_ORDER);


    database = nullptr;

    if (!EasyHamLog::QSODatabaseInterface::readDatabase("prefix_xml.xml", database, &root)) {
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

    delete database;

}

EasyHamLog::MainUIApplication::~MainUIApplication()
{
    for (auto& qso : registeredQSOs) {
        delete qso;
    }
    for (auto& prefix : callsignPrefixes) {
        delete prefix;
    }
    delete ui;
}

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

EasyHamLog::Callsign_Prefix* EasyHamLog::MainUIApplication::getPrefix(const QString& call_prefix) {
    for (auto& prefix : callsignPrefixes) {
        std::vector<std::string> splitted = splitString('-', prefix->prefix);

        if (splitted.size() == 1) {
            if (splitted[0] == std::string(call_prefix.toStdString())) {
                return prefix;
            }
        }
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

    EasyHamLog::QSOAddDialog addDialog(this);
    addDialog.setModal(true);
    int ret = addDialog.exec();     // 1 = Ok, 0 = Cancel
    
    if (ret == 1) {

        EasyHamLog::QSO* qso = addDialog.getQSO();

        registeredQSOs.push_back(qso);

        EasyHamLog::QSODatabaseInterface::writeDatabase("database.xml", registeredQSOs);

        insertRowData(ui->tableWidget, 0, qso);

        ui->tableWidget->sortItems(2, MAIN_SORT_ORDER);
        ui->tableWidget->sortItems(3, MAIN_SORT_ORDER);

    }
}

void EasyHamLog::MainUIApplication::on_findContactEdit_textEdited(const QString &arg1)
{
    QList<QTableWidgetItem*> tempTable = ui->tableWidget->findItems(arg1, Qt::MatchContains);
    
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->setRowHidden(i, !tempTable.contains(ui->tableWidget->item(i, 0)));
    }
}

void EasyHamLog::MainUIApplication::on_actionQuit_triggered()
{
    QApplication::quit();
}

void EasyHamLog::MainUIApplication::setRowData(QTableWidget* table, int row, EasyHamLog::QSO* qso)
{
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
    table->insertRow(row);
    
    QString uuid = QUuid::createUuid().toString();
    qsoRows[uuid] = qso;

    setRowData(table, row, qso);

    table->setItem(row, table->columnCount() - 1, new QTableWidgetItem(uuid));
}


void EasyHamLog::MainUIApplication::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    EasyHamLog::QSO* qso = qsoRows[ui->tableWidget->item(item->row(), ui->tableWidget->columnCount() - 1)->text()];
    EasyHamLog::QSOAddDialog addDialog(this, qso);
    addDialog.setModal(true);
    int ret = addDialog.exec();


    if (ret) {
        
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

        setRowData(ui->tableWidget, rowIndex, qso);

        EasyHamLog::QSODatabaseInterface::writeDatabase("database.xml", registeredQSOs);

    }
}
