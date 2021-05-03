#include "qsomap.h"
#include "ui_qsomap.h"
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <QQuickItem>

QSOMap::QSOMap(std::vector<EasyHamLog::QSO*>& qsos, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSOMap)
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl::fromLocalFile("../forms/map.qml"));
    ui->quickWidget->show();
    
    QQuickItem* root = ui->quickWidget->rootObject();

    QVariant returned;

    for (auto& qso : qsos) {

        QVariantMap qso_list;

        qso_list.insert("callsign", qso->callsign.c_str());
        qso_list.insert("name" , qso->name.c_str());
        qso_list.insert("opmode" , qso->opmode.c_str());
        qso_list.insert("band" , qso->band.c_str());
        qso_list.insert("freq" , qso->freq.c_str());
        qso_list.insert("locator" , qso->locator.c_str());

        QMetaObject::invokeMethod(root, "addQSOMarker", Q_RETURN_ARG(QVariant, returned), Q_ARG(QVariant, QVariant::fromValue(qso_list)));

    }

}

QSOMap::~QSOMap()
{
    delete ui;
}

void QSOMap::emitAddQSO(EasyHamLog::QSO* qso)
{
    emit doAddQSO(qso);
}
