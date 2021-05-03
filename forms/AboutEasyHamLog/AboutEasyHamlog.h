#ifndef ABOUTEASYHAMLOG_H
#define ABOUTEASYHAMLOG_H

#include <QDialog>

namespace Ui {
class AboutEasyHamLog;
}

class AboutEasyHamLog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutEasyHamLog(QString version, int app_version_encoded, QWidget *parent=nullptr);
    ~AboutEasyHamLog();

private:
    Ui::AboutEasyHamLog *ui;
};

#endif // ABOUTEASYHAMLOG_H
