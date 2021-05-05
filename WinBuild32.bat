@echo off
echo Building EasyHamLog...

echo Creating directories...
mkdir build

echo Configuring CMake with VS2019
cmake -S. -Bbuild -G "Visual Studio 16 2019" -
cmake --build build --config Release

echo
echo
echo Copying config files...

mkdir build/
copy tools/prefix_lookup.xml build/

echo
echo
echo Executable in build/Win32/Release