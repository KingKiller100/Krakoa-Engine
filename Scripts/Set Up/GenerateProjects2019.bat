@ECHO off
path = %~dp0..\..\%
pushd %path%
IF EXIST "%path%.vs\" (
	del "%path%.vs\" /f /s /q
	rmdir "%path%.vs\" /s /q
)
IF EXIST "%path%bin\" (
	del "%path%bin\" /f /s /q
	del "%path%bin-int\" /f /s /q
	rmdir "%path%bin\" /s /q
	rmdir "%path%bin-int\" /s /q
	)
IF EXIST "%path%\kLibrary\" (
	IF EXIST "%path%\kLibrary\bin\" (
		del "%path%\kLibrary\bin\" /f /s /q
		del "%path%\kLibrary\bin-int\" /f /s /q
		rmdir "%path%\kLibrary\bin\" /s /q
		rmdir "%path%\kLibrary\bin-int\" /s /q
	)
	IF EXIST "%path%\kLibrary\Include Files" (
		del "%path%\kLibrary\Include Files" /f /s /q
	)
)
call Vendors\bin\premake\premake5.exe vs2019
popd
PAUSE