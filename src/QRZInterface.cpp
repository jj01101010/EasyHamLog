#include "..\include\QRZInterface.h"

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

	if (pendingRequests.front() == EasyHamLog::QRZInterfaceRequestType::LOGIN_REQUEST) {
		pendingRequests.pop();

		QDomNodeList qrzSessionElements = qrzDatabaseElement.elementsByTagName("Session");
		if (qrzSessionElements.count() <= 0) {
			return;
		}

		QDomElement session = qrzSessionElements.at(0).toElement();
		QDomNodeList sessionErrors = session.elementsByTagName("Error");
		if (sessionErrors.count() > 0) {
			qInfo() << "QRZ Error: " << sessionErrors.at(0).toElement().text();
			QRZInterface::getOpenQRZInterface()->isLoggedIn = false;
			return;
		}
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
	qInfo() << isLoggedIn;
	return isLoggedIn && sessionKey != "";
}

bool EasyHamLog::QRZInterface::HasPendingRequests() const
{
	return pendingRequests.size() != 0;
}
