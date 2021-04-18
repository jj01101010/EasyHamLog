#ifndef QSOADDDIALOG_H
#define QSOADDDIALOG_H

#include <QDialog>
#include <mainuiapplication.h>

namespace Ui {
class QSOAddDialog;
}

namespace HamLog {

    class QSOAddDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit QSOAddDialog(HamLog::MainUIApplication* parent = nullptr, HamLog::QSO* edited = nullptr);
        ~QSOAddDialog();

        HamLog::QSO* getQSO() const;

    private slots:
        void on_fnButton_clicked();

        void on_fnnButton_clicked();

        void on_callsignEdit_editingFinished();

    private:
        HamLog::MainUIApplication* parent;
        Ui::QSOAddDialog* ui;
    };

}

#endif // QSOADDDIALOG_H
