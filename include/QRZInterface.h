#pragma once

#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <string>
#include <QtXml>
#include <queue>
#include <QSO.h>

namespace EasyHamLog {

	enum class QRZInterfaceRequestType {
		LOGIN_REQUEST = 0,
		DATABASE_REQUEST
	};

	class QRZInterface : public QObject {

		Q_OBJECT
	public:
		static QRZInterface* getOpenQRZInterface();
		
		void deleteInterface();
		
		void login(std::string uname, std::string pwd);

		bool HasLoggedIn() const;

		bool HasPendingRequests() const;

		EasyHamLog::QSO* findQSOInQRZ(std::string callsign);

	private:

		QRZInterface();

		void networkManagerFinishedCallback(QNetworkReply* reply);

	private:
		static QRZInterface* instance;

		QDomElement callsignReply;

		const std::string qrz_host_name = "http://xmldata.qrz.com/xml/current/";

		bool isLoggedIn = false;
		std::string sessionKey;

		QNetworkAccessManager* manager;
		QNetworkRequest request;

		std::queue<QRZInterfaceRequestType> pendingRequests;

	};
}
