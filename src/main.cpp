#include <qapplication.h>
#include <mainuiapplication.h>

int main(int argc, char** argv) {
	QApplication qt_app(argc, argv);
	EasyHamLog::MainUIApplication* window = new EasyHamLog::MainUIApplication();
	window->show();
	if (window->setupSuccess) {
		return qt_app.exec();
	} else {
		return 1;
	}
}
