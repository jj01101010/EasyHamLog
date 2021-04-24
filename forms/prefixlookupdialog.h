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
        explicit PrefixLookupDialog(QWidget* parent=nullptr);
        ~PrefixLookupDialog();

    private slots:
        void on_lineEdit_textEdited(const QString& line);

    private:
        Ui::PrefixLookupDialog* ui;
    };
}


#endif // PREFIXLOOKUPDIALOG_H
