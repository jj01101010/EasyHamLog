@echo off
echo Building EasyHamLog...

echo Creating directories
mkdir build

echo Configuring CMake with VS2019
cmake -S. -Bbuild -G "Visual Studio 16 2019"
cmake --build build --config Release

echo Copying some other files

mkdir build\Release\lookups
copy tools\prefix_lookup.xml build\Release\lookups\prefix_lookup.xml

echo
echo
echo Executable in build/Release