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

namespace HamLog {

    class MainUIApplication : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainUIApplication(QWidget* parent = nullptr);
        ~MainUIApplication();
        bool setupSuccess = true;

        HamLog::Callsign_Prefix* getPrefix(const QString& name);    // TODO: Move to different location
        
    private slots:
        void on_addContactButton_clicked();
        void on_findContactEdit_textEdited(const QString& arg1);

        void on_actionQuit_triggered();

        void on_tableWidget_itemDoubleClicked(QTableWidgetItem* item);

    private:
        void setRowData(QTableWidget* table, int row, HamLog::QSO* qso);
        void insertRowData(QTableWidget* table, int row, HamLog::QSO* qso);

        std::vector<std::string> splitString(const char del, const std::string& s);

    private:
        std::vector<HamLog::QSO*> registeredQSOs;
        std::vector<HamLog::Callsign_Prefix*> callsignPrefixes;
        std::unordered_map<QString, HamLog::QSO*> qsoRows;

        Ui::MainUIApplication* ui;
    };

}

#endif // MAINUIAPPLICATION_H
