@ECHO off
path = %~dp0..\..\%
pushd %path%
IF EXIST "%path%bin\" (
	del "%path%bin\" /f /s /q
	del "%path%bin-int\" /f /s /q
	rmdir "%path%bin\" /s /q
	rmdir "%path%bin-int\" /s /q
	)
popd