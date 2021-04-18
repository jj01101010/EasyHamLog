#ifndef QSOADDDIALOG_H
#define QSOADDDIALOG_H

#include <QDialog>
#include <mainuiapplication.h>

namespace Ui {
class QSOAddDialog;
}

namespace EasyHamLog {

    class QSOAddDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit QSOAddDialog(EasyHamLog::MainUIApplication* parent = nullptr, EasyHamLog::QSO* edited = nullptr);
        ~QSOAddDialog();

        EasyHamLog::QSO* getQSO() const;

    private slots:
        void on_fnButton_clicked();

        void on_fnnButton_clicked();

        void on_callsignEdit_editingFinished();

    private:
        EasyHamLog::MainUIApplication* parent;
        Ui::QSOAddDialog* ui;
    };

}

#endif // QSOADDDIALOG_H
