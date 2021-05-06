@echo off

if "%QTDIR%" == "" (
	echo "ERROR: QTDIR Is not defined!"
	echo You need to create a system variable called "QTDIR"
	echo and set it ot the cmake install path [PATH/QT_VERSION/COMPILER_YOU_ARE_USING]
	echo After you created the system variable, create a new Path to "%QTDIR%\bin" [Used for linking with the DLLs].
	echo You could also copy the needed DLLs from "QT_PATH/QT_VERSION/bin" and the needed plugins from "QT_PATH/QT_VERSION/plugins" [See Github page for more information]
	pause
	exit 0
)

echo Building EasyHamLog...
echo Creating directories...
mkdir build

echo Configuring CMake with VS2019
cmake -S. -Bbuild -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release

echo Copying config files...

mkdir build\\Release\\lookups
copy tools\\prefix_lookup.xml build\\Release\\lookups\\prefix_lookup.xml

echo Executable in build/Release
pause