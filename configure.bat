@echo off

set VC_HOME="F:\Microsoft Visual Studio 12.0\VC"

call %VC_HOME%"\bin\vcvars32.bat"
call cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug
