#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSO.h>

namespace Ui {
class PreferencesDialog;
}

namespace EasyHamLog {
    class PreferencesDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit PreferencesDialog(EasyHamLog::Preferences* preferences, QWidget* parent = nullptr);
        ~PreferencesDialog();

        EasyHamLog::Preferences* getPreferences() const;

    private:
        Ui::PreferencesDialog* ui;
    };
}

#endif // PREFERENCESDIALOG_H
