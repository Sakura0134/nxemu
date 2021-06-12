@echo off
SETLOCAL

set origdir=%cd%
cd /d %~dp0..\..
set base_dir=%cd%
cd /d %origdir%

if "%~3"=="" (
    echo Usage: UpdateVersion.cmd [BuildMode] [InFile] [OutFile]
    goto :eof
)

if not "%1" == "" set BuildMode=%~1
if not "%~2" == "" set InFile="%~2"
if not "%~3" == "" set OutFile="%~3"

echo "%base_dir%\Bin\%BuildMode%\UpdateVersion.exe" %InFile% %OutFile%
"%base_dir%\Bin\%BuildMode%\UpdateVersion.exe" %InFile% %OutFile%
