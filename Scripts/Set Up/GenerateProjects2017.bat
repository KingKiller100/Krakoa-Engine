@echo off
path = %~dp0..\..\%
pushd %path%
IF EXIST "%path%bin\" (rmdir "%path%bin\" /s /q)
IF EXIST "%path%bin-int\" (rmdir "%path%bin-int\" /s /q)
call Vendors\bin\premake\premake5.exe vs2017
popd
PAUSE