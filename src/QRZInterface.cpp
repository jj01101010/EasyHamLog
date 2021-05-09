#include "..\include\QRZInterface.h"
#include <QMessageBox>

EasyHamLog::QRZInterface* EasyHamLog::QRZInterface::instance = 0;

EasyHamLog::QRZInterface::QRZInterface()
{
	manager = new QNetworkAccessManager();
	QObject::connect(manager, &QNetworkAccessManager::finished, this, &EasyHamLog::QRZInterface::networkManagerFinishedCallback);
}

void EasyHamLog::QRZInterface::networkManagerFinishedCallback(QNetworkReply* net_reply)
{
	if (net_reply->error()) {
		qDebug() << net_reply->errorString();
		pendingRequests.pop();
		return;
	}

	QString reply = net_reply->readAll();

	QDomDocument* replyDocument = new QDomDocument;
	replyDocument->setContent(reply);
	QDomElement qrzDatabaseElement = replyDocument->firstChildElement();
	qInfo() << qrzDatabaseElement.text();

	QDomNodeList qrzSessionElements = qrzDatabaseElement.elementsByTagName("Session");
	if (qrzSessionElements.count() <= 0) {
		return;
	}
	QDomElement session = qrzSessionElements.at(0).toElement();

	QDomNodeList sessionErrors = session.elementsByTagName("Error");
	if (sessionErrors.count() > 0) {
		qInfo() << "QRZ Error: " << sessionErrors.at(0).toElement().text();
		pendingRequests.pop();
		return;
	}


	if (pendingRequests.front() == EasyHamLog::QRZInterfaceRequestType::LOGIN_REQUEST) {
		pendingRequests.pop();

		QDomNodeList sessionKeys = session.elementsByTagName("Key");
		if (sessionKeys.count() > 0) {
			QRZInterface::getOpenQRZInterface()->sessionKey = sessionKeys.at(0).toElement().text().toStdString();
			if (QRZInterface::getOpenQRZInterface()->sessionKey != "") {
				QRZInterface::getOpenQRZInterface()->isLoggedIn = true;
				qInfo() << QRZInterface::getOpenQRZInterface()->sessionKey.c_str() << QRZInterface::getOpenQRZInterface()->isLoggedIn;
			}
		}
	}
	else if (pendingRequests.front() == EasyHamLog::QRZInterfaceRequestType::DATABASE_REQUEST) {
		pendingRequests.pop();

		QDomNodeList callsignKeys = qrzDatabaseElement.elementsByTagName("Callsign");
		if (callsignKeys.count() <= 0) {
			return;
		}
		callsignReply = callsignKeys.at(0).toElement();
	}
}

EasyHamLog::QRZInterface* EasyHamLog::QRZInterface::getOpenQRZInterface()
{
	if (instance == 0) {
		instance = new QRZInterface();
	}
	return instance;
}


void EasyHamLog::QRZInterface::deleteInterface() {
	delete manager;
}

void EasyHamLog::QRZInterface::login(std::string uname, std::string pwd)
{
	if (!isLoggedIn) {
		pendingRequests.push(EasyHamLog::QRZInterfaceRequestType::LOGIN_REQUEST);
		manager->get(QNetworkRequest(QUrl((qrz_host_name + "?username=" + uname + ";password=" + pwd).c_str())));
	}
}

bool EasyHamLog::QRZInterface::HasLoggedIn() const
{
	return isLoggedIn && sessionKey != "";
}

bool EasyHamLog::QRZInterface::HasPendingRequests() const
{
	return pendingRequests.size() != 0;
}

EasyHamLog::QSO* EasyHamLog::QRZInterface::findQSOInQRZ(std::string callsign)
{
	if (isLoggedIn) {
		pendingRequests.push(EasyHamLog::QRZInterfaceRequestType::DATABASE_REQUEST);
		QNetworkReply* reply = manager->get(QNetworkRequest(QUrl((qrz_host_name + "?s=" + sessionKey + ";callsign=" + callsign).c_str())));
		
		QEventLoop loop;
		QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
		loop.exec();

		if (callsignReply.isNull()) {
			return nullptr;
		}

		EasyHamLog::QSO* ham_member = new EasyHamLog::QSO;
		
		ham_member->name = callsignReply.elementsByTagName("fname").count() > 0 ? callsignReply.elementsByTagName("fname").at(0).toElement().text().toStdString() : "";
		ham_member->locator = callsignReply.elementsByTagName("grid").count() > 0 ? callsignReply.elementsByTagName("grid").at(0).toElement().text().toStdString() : "";
		ham_member->country = callsignReply.elementsByTagName("country").count() > 0 ? callsignReply.elementsByTagName("country").at(0).toElement().text().toStdString() : "";

		return ham_member;
	}
	return nullptr;
}
