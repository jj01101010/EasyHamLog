#ifndef PREFIXLOOKUPDIALOG_H
#define PREFIXLOOKUPDIALOG_H

#include <QDialog>

namespace EasyHamLog {
    class MainUIApplication;
}

namespace Ui {
class PrefixLookupDialog;
}

namespace EasyHamLog {
    class PrefixLookupDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit PrefixLookupDialog(EasyHamLog::MainUIApplication* parent);
        ~PrefixLookupDialog();

    private slots:
        void on_lineEdit_textEdited(const QString& line);

    private:
        EasyHamLog::MainUIApplication* parent;
        Ui::PrefixLookupDialog* ui;
    };
}


#endif // PREFIXLOOKUPDIALOG_H
