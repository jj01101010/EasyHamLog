#ifndef QSOADDDIALOG_H
#define QSOADDDIALOG_H

#include <QDialog>
#include <mainuiapplication.h>

namespace Ui {
class QSOAddDialog;
}

class QSOAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSOAddDialog(MainUIApplication* parent=nullptr);
    ~QSOAddDialog();

private slots:
    void on_fnButton_clicked();

    void on_fnnButton_clicked();

    void on_callsignEdit_editingFinished();

private:
    MainUIApplication* parent;
    Ui::QSOAddDialog *ui;
};

#endif // QSOADDDIALOG_H
