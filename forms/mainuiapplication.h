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
private slots:
    void on_addContactButton_clicked();
    void on_findContactEdit_textEdited(const QString &arg1);

    void on_actionQuit_triggered();

private:
    void insertRowData(QTableWidget* table, int row, HamLog::QSO* qso);

private:
    std::vector<HamLog::QSO*> registeredQSOs;

    Ui::MainUIApplication *ui;
};

#endif // MAINUIAPPLICATION_H
