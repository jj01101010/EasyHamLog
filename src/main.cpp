#include <qapplication.h>
#include <mainuiapplication.h>

int main(int argc, char** argv) {
	QApplication qt_app(argc, argv);
	HamLog::MainUIApplication* window = new HamLog::MainUIApplication();
	window->show();
	if (window->setupSuccess) {
		return qt_app.exec();
	} else {
		return 1;
	}
}
