#include <ADIInterface.h>
#include <QFile>
#include <QDateTime>

bool EasyHamLog::ADIInterface::writeADIFile(const QString& filename, std::vector<EasyHamLog::QSO*> qsos)
{
	QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}
	
	// Write Header
	file.write(QString("Generated on %1 at %2\n").arg(QDate::currentDate().toString("yyyy-MM-dd")).arg(QTime::currentTime().toString("hh:mm:ss")).toStdString().c_str());
	file.write("<adif_ver:5>3.0.5\n");
	file.write("<programid:10>EasyHamLog\n");
	file.write("<EOH>\n\n");

	for (auto& qso : qsos) {
		file.write(QString("<CALL:%1>%2\n").arg(qso->callsign.size()).arg(qso->callsign.c_str()).toStdString().c_str());
		file.write(QString("<BAND:%1>%2\n").arg(qso->band.size()).arg(qso->band.c_str()).toStdString().c_str());
		file.write(QString("<COUNTRY:%1>%2\n").arg(qso->country.size()).arg(qso->country.c_str()).toStdString().c_str());
		file.write(QString("<FREQ:%1>%2\n").arg(qso->freq.size()).arg(qso->freq.c_str()).toStdString().c_str());
		file.write(QString("<MODE:%1>%2\n").arg(qso->opmode.size()).arg(qso->opmode.c_str()).toStdString().c_str());
		file.write(QString("<NAME:%1>%2\n").arg(qso->name.size()).arg(qso->name.c_str()).toStdString().c_str());
		file.write(QString("<RST_SENT:%1>%2\n").arg(qso->rst.size()).arg(qso->rst.c_str()).toStdString().c_str());
		file.write(QString("<GRIDSQUARE:%1>%2\n").arg(qso->locator.size()).arg(qso->locator.c_str()).toStdString().c_str());

		QDate tempDate = QDate::fromString(qso->date.c_str(), "dd.MM.yyyy ddd");
		QTime tempTime = QTime::fromString(qso->time.c_str());
		
		QString tempDateString = tempDate.toString("yyyyMMdd");
		QString tempTimeString = tempTime.toString("hhmm");

		file.write(QString("<QSO_DATE:%1>%2\n").arg(tempDateString.size()).arg(tempDateString).toStdString().c_str()); 
		file.write(QString("<time_on:%1>%2\n").arg(tempTimeString.size()).arg(tempTimeString).toStdString().c_str());
		file.write("<EOR>\n\n");
	}

	file.close();

	return false;
}
