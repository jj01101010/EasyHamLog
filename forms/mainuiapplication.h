#ifndef MAINUIAPPLICATION_H
#define MAINUIAPPLICATION_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QTableWidget>

#include <QSO.h>

namespace Ui {
class MainUIApplication;
}

class MainUIApplication : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUIApplication(QWidget *parent = nullptr);
    ~MainUIApplication();
    bool setupSuccess = true;

    HamLog::Callsign_Prefix* getPrefix(const QString& name);

private slots:
    void on_addContactButton_clicked();
    void on_findContactEdit_textEdited(const QString &arg1);

    void on_actionQuit_triggered();

private:
    void insertRowData(QTableWidget* table, int row, HamLog::QSO* qso);

    std::vector<std::string> splitString(const char del, const std::string& s);

private:
    std::vector<HamLog::QSO*> registeredQSOs;
    std::vector<HamLog::Callsign_Prefix*> callsignPrefixes;

    Ui::MainUIApplication *ui;
};

#endif // MAINUIAPPLICATION_H
