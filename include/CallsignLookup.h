#pragma once

#include <vector>
#include <QSO.h>
#include <QString>

namespace EasyHamLog {
	class CallsignLookup {
	public:
		static bool Initialize();
		static void Destroy();
		static EasyHamLog::Callsign_Prefix* getPrefix(const QString& name);

	private:
		static std::vector<EasyHamLog::Callsign_Prefix*> prefixes;
	};
}
