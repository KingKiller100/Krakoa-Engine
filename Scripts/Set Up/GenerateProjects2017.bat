@ECHO off
pushd ..\..\
call Scripts\EraseBin\EraseBin.bat
call Vendors\bin\premake\premake5.exe vs2017
popd
PAUSE