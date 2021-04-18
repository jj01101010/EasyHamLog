#include <QSODatabaseInterface.h>

bool EasyHamLog::QSODatabaseInterface::readDatabase(const QString& name, QSO_DATABASE* database, QSO_DATABASE_ELEMENT* root, bool create)
{
    QSO_DATABASE* qso_database = new QSO_DATABASE;
    QFile file(name);

    bool fileExisted = file.exists();

    enum QIODevice::OpenModeFlag flag = create ? QIODevice::ReadWrite : QIODevice::ReadOnly;

    if (!file.open(flag | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Loading File Error", "Could not load " + name, QMessageBox::Ok, QMessageBox::NoButton);
        delete qso_database;
        return false;
    }

    database = qso_database;

    if (fileExisted) {
        // Get the first child Entry from the DATABASE tag
        database->setContent(&file);
        *root = database->documentElement().firstChildElement();
    }
    else {
        // Create an empty database with the DATABASE Root tag
        QSO_DATABASE_ELEMENT document_root = database->createElement("DATABASE");
        database->appendChild(document_root);
        *root = document_root.firstChildElement();

        QTextStream stream(&file);
        stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";   // Add xml header
        stream << database->toString();                             // Write the empty database
    }

    file.close();
    return true;
}

EasyHamLog::QSO* EasyHamLog::QSODatabaseInterface::nextQSO(QSO_DATABASE_ELEMENT* root)
{
    QSO_DATABASE_ELEMENT element = nextElement(root);
    if (element.isNull()) {
        return nullptr;
    }
    auto qso = new EasyHamLog::QSO;
    qso->name = element.attribute("name", "").toStdString();
    qso->callsign = element.attribute("callsign", "").toStdString();
    qso->time = element.attribute("time", "").toStdString();
    qso->date = element.attribute("date", "").toStdString();
    qso->opmode = element.attribute("opmode", "").toStdString();
    qso->band = element.attribute("band", "").toStdString();
    qso->freq = element.attribute("freq", "").toStdString();
    qso->rst = element.attribute("rst", "").toStdString();
    qso->locator = element.attribute("locator", "").toStdString();
    qso->country = element.attribute("country", "").toStdString();
    
    return qso;
}

bool EasyHamLog::QSODatabaseInterface::writeDatabase(const QString& name, std::vector<EasyHamLog::QSO*> qsos) {

    QFile file(name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Writing File Error", "Could not load database.xml", QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }

    QSO_DATABASE database;
    QSO_DATABASE_ELEMENT document_root = database.createElement("DATABASE");
    
    for (auto& qso : qsos) {
        QDomElement qso_element = database.createElement("QSO");
        qso_element.setAttribute("name", qso->name.c_str());
        qso_element.setAttribute("callsign", qso->callsign.c_str());
        qso_element.setAttribute("time", qso->time.c_str());
        qso_element.setAttribute("date", qso->date.c_str());
        qso_element.setAttribute("band", qso->band.c_str());
        qso_element.setAttribute("opmode", qso->opmode.c_str());
        qso_element.setAttribute("freq", qso->freq.c_str());
        qso_element.setAttribute("rst", qso->rst.c_str());
        qso_element.setAttribute("locator", qso->locator.c_str());
        qso_element.setAttribute("country", qso->country.c_str());

        document_root.appendChild(qso_element);
    }

    database.appendChild(document_root);
    QTextStream stream(&file);
    stream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";   // Add xml header
    stream << database.toString();                             // Write the empty database

    file.close();
    return true;
}

QSO_DATABASE_ELEMENT EasyHamLog::QSODatabaseInterface::nextElement(QSO_DATABASE_ELEMENT* root) {
    if (root->isNull()) {
        return QDomElement();
    }
    
    QSO_DATABASE_ELEMENT element = *root;

    *root = root->nextSiblingElement();     // Set root to the next sibbling of the QSO List
    return element;
}
