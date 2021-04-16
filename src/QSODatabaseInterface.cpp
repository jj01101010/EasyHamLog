#include <QSODatabaseInterface.h>

bool HamLog::QSODatabaseInterface::readDatabase(const QString& name, QSO_DATABASE* qso_database, QSO_DATABASE_ELEMENT* root)
{
    QSO_DATABASE* database = new QSO_DATABASE;

    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Loading File Error", "Could not load database.xml", QMessageBox::Ok, QMessageBox::NoButton);
        delete database;
        return false;
    }

    database->setContent(&file);
    file.close();

    qso_database = database;
    *root = qso_database->documentElement().firstChildElement();

    return true;
}

HamLog::QSO* HamLog::QSODatabaseInterface::nextQSO(QSO_DATABASE_ELEMENT* root)
{
    if (root->isNull()) {
        return nullptr;
    }
    auto qso = new HamLog::QSO;
    qso->name       = root->attribute("name", "").toStdString();
    qso->callsign   = root->attribute("callsign", "").toStdString();
    qso->time       = root->attribute("time", "").toStdString();
    qso->date       = root->attribute("date", "").toStdString();
    qso->band       = root->attribute("band", "").toStdString();
    qso->freq       = root->attribute("freq", "").toStdString();
    qso->locator    = root->attribute("locator", "").toStdString();
    qso->country    = root->attribute("country", "").toStdString();

    *root = root->nextSiblingElement();
    return qso;
}
