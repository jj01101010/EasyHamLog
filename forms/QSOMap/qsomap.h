#ifndef QSOMAP_H
#define QSOMAP_H

#include <QDialog>
#include <QSO.h>
#include <vector>

namespace Ui {
class QSOMap;
}

class QSOMap : public QDialog
{
    Q_OBJECT

public:
    explicit QSOMap(std::vector<EasyHamLog::QSO*>& qsos, QWidget* parent = nullptr);
    ~QSOMap();

signals:
    void doAddQSO(EasyHamLog::QSO* qso);

private:

    void emitAddQSO(EasyHamLog::QSO* qso);

private:
    Ui::QSOMap* ui;
};

#endif // QSOMAP_H
