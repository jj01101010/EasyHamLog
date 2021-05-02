#ifndef MAINUIAPPLICATION_H
#define MAINUIAPPLICATION_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QTableWidget>
#include <unordered_map>

#include <QSO.h>

namespace Ui {
class MainUIApplication;
}

namespace EasyHamLog {

    class MainUIApplication : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainUIApplication(QWidget* parent = nullptr);
        ~MainUIApplication();
        bool setupSuccess = true;

        EasyHamLog::QSO* findQSOByCallsign(const std::string& callsign) const;

    private slots:
        void on_addContactButton_clicked();
        void on_findContactEdit_textEdited(const QString& arg1);

        void on_actionQuit_triggered();

        void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);

        void on_actionPrefix_Lookup_triggered();

        void on_actionExport_Database_triggered();

        void on_actionNew_Session_triggered();

        void on_actionOpen_Session_triggered();

    private:
        void setRowData(QTableWidget* table, int row, EasyHamLog::QSO* qso);
        void insertRowData(QTableWidget* table, int row, EasyHamLog::QSO* qso);

        void newSession();

    private:
        QString databasePath;
        std::vector<EasyHamLog::QSO*> registeredQSOs;
        std::unordered_map<std::string, EasyHamLog::QSO*> qsoRows;

        Ui::MainUIApplication* ui;
    };

}

#endif // MAINUIAPPLICATION_H
