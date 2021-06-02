#ifndef QSOADDDIALOG_H
#define QSOADDDIALOG_H

#include <QDialog>
#include <MainUIApplication/mainuiapplication.h>

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
        explicit QSOAddDialog(EasyHamLog::MainUIApplication* parent, QString myLocator, EasyHamLog::QSO* edited = nullptr);
        ~QSOAddDialog();

        EasyHamLog::QSO* getQSO() const;

    private slots:
        void on_fnButton_clicked();

        void on_fnnButton_clicked();

        void on_callsignEdit_editingFinished();

        void on_deleteButton_clicked();

        void on_saveButton_clicked();

        void on_cancelButton_clicked();

        void on_addContestInfoButton_clicked();

        void on_locatorEdit_textChanged(const QString &arg1);

    private:
        void toggleContest(bool change_value=true);

    private:

        bool contest;
        EasyHamLog::MainUIApplication* parent;
        Ui::QSOAddDialog* ui;
        QString myLocator;

    };

}

#endif // QSOADDDIALOG_H
