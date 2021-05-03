#include <QApplication>
#include <MainUIApplication/mainuiapplication.h>

int main(int argc, char** argv) {

	// Instantiate main QApplication
	QApplication qt_app(argc, argv);

	// Instantiate the main UI window
	EasyHamLog::MainUIApplication* window = new EasyHamLog::MainUIApplication();
	window->show();	// show the UI window
	// Boolean set by the constructor of the MainUIApplication. If this is false some error happend
	// while setting up
	if (window->setupSuccess) {
		return qt_app.exec();	// Execute the app
	} else {
		return 1;
	}
}
