#pragma once

#include <vector>
#include <QSO.h>
#include <QFile>
#include <QtXml>
#include <QMessageBox>

namespace HamLog {
	
	#define QSO_DATABASE_ELEMENT QDomElement
	#define QSO_DATABASE QDomDocument

	class QSODatabaseInterface {
	public:

		static bool readDatabase(const QString& name, QSO_DATABASE* document, QSO_DATABASE_ELEMENT* root);

		static QSO* nextQSO(QSO_DATABASE_ELEMENT* root);
	};
}
