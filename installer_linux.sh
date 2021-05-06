if [[ "$OSTYPE" != "linux-gnu" ]]; then
	echo "This installer is only compatable with linux environments!"
	read
	exit 1
fi

if [[ "$(whoami)" != root ]]; then
	echo "Please execute the script as root!"
	read
	exit 1
fi

echo Installing dependencies...
sudo apt-get install make cmake build-essentials qtbase5-dev qtdeclarative5-dev

echo Configuring CMake project...
mkdir build
cmake -S. -Bbuild

echo Building project...
cmake --build build

echo Copying prefix table...
mkdir build/lookups
cp tools/prefix_lookup.xml build/lookups/prefix_lookup.xml
