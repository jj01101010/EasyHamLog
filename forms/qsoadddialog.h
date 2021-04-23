#ifndef QSOADDDIALOG_H
#define QSOADDDIALOG_H

#include <QDialog>
#include <mainuiapplication.h>

#define QSO_ADD_DIALOG_RESULT_CANCEL 0
#define QSO_ADD_DIALOG_RESULT_SAVE   1
#define QSO_ADD_DIALOG_RESULT_DELETE 2

namespace Ui {
class QSOAddDialog;
}

namespace EasyHamLog {

    class QSOAddDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit QSOAddDialog(EasyHamLog::MainUIApplication* parent, EasyHamLog::QSO* edited = nullptr);
        ~QSOAddDialog();

        EasyHamLog::QSO* getQSO() const;

    private slots:
        void on_fnButton_clicked();

        void on_fnnButton_clicked();

        void on_callsignEdit_editingFinished();

        void on_deleteButton_clicked();

        void on_saveButton_clicked();

        void on_cancelButton_clicked();

    private:
        EasyHamLog::MainUIApplication* parent;
        Ui::QSOAddDialog* ui;
    };

}

#endif // QSOADDDIALOG_H
