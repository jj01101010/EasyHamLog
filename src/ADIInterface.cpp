#include <ADIInterface.h>
#include <QFile>
#include <QDateTime>

#define CREATE_ADI_FILE_STRING(string, var) QString(string).arg(var.size()).arg(var.c_str()).toStdString().c_str()

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
		file.write(CREATE_ADI_FILE_STRING("<CALL:%1>%2\n", qso->callsign));
		file.write(CREATE_ADI_FILE_STRING("<BAND:%1>%2\n", qso->band));
		file.write(CREATE_ADI_FILE_STRING("<COUNTRY:%1>%2\n", qso->country));
		file.write(CREATE_ADI_FILE_STRING("<FREQ:%1>%2\n", qso->freq));
		file.write(CREATE_ADI_FILE_STRING("<MODE:%1>%2\n", qso->opmode));
		file.write(CREATE_ADI_FILE_STRING("<NAME:%1>%2\n", qso->name));
		file.write(CREATE_ADI_FILE_STRING("<RST_SENT:%1>%2\n", qso->rst));
		file.write(CREATE_ADI_FILE_STRING("<GRIDSQUARE:%1>%2\n", qso->locator));

		if (qso->contest_number != "" || qso->contest_info != "") {
			file.write(CREATE_ADI_FILE_STRING("<SRX_STRING:%1>%2\n", qso->contest_info));
			file.write(CREATE_ADI_FILE_STRING("<SRX:%1>%2\n", qso->contest_number));
		}
		
		QDate tempDate = QDate::fromString(qso->date.c_str(), "dd.MM.yyyy ddd");
		QTime tempTime = QTime::fromString(qso->time.c_str());
		
		QString tempDateString = tempDate.toString("yyyyMMdd");
		QString tempTimeString = tempTime.toString("hhmm");

		file.write(CREATE_ADI_FILE_STRING("<QSO_DATE:%1>%2\n", tempDateString.toStdString()));
		file.write(CREATE_ADI_FILE_STRING("<TIME_ON:%1>%2\n", tempTimeString.toStdString()));
		file.write("<EOR>\n\n");
	}

	file.close();

	return false;
}
