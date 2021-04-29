#pragma once

#include <vector>
#include <QSO.h>
#include <QString>

namespace EasyHamLog {
	class ADIInterface {
	public:
		static bool writeADIFile(const QString& filename, std::vector<EasyHamLog::QSO*> qsos);
	};
}
