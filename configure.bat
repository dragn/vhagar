@echo off

pushd
mkdir build
cd build
..\..\vhagar-deps\cmake-3.9.2-win64-x64\bin\cmake.exe .. -G "Visual Studio 14 2015 Win64"
popd