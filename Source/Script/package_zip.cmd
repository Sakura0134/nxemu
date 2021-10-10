@ECHO OFF
SETLOCAL

for /f "delims=" %%a in ('WHERE 7z 2^>nul') do set "zip=%%a"

if "%zip%" == "" (
    if exist "C:\Program Files\7-Zip\7z.exe" (
        set "zip=C:\Program Files\7-Zip\7z.exe"
    ) else (
        echo can not find 7z.exe
        goto :EndErr
    )
)

set ZipFileName=NXEmu
if not "%1" == "" set ZipFileName=%1


SET current_dir=%cd%
cd /d %~dp0..\..\
SET base_dir=%cd%
cd /d %current_dir%

IF EXIST "%base_dir%\Package" rmdir /S /Q "%base_dir%\Package"
IF %ERRORLEVEL% NEQ 0 GOTO EndErr
IF NOT EXIST "%base_dir%\Package" mkdir "%base_dir%\Package"
IF %ERRORLEVEL% NEQ 0 GOTO EndErr

rd "%base_dir%\Bin\Package" /Q /S > NUL 2>&1
md "%base_dir%\Bin\Package"
md "%base_dir%\Bin\Package\Config"
md "%base_dir%\Bin\Package\Plugin"
md "%base_dir%\Bin\Package\Plugin\Video"

copy "%base_dir%\Bin\Release64\nxemu.exe" "%base_dir%\Bin\Package"
copy "%base_dir%\Plugin\Video\NXEmu-Video.dll" "%base_dir%\Bin\Package\Plugin\Video"

cd %base_dir%\Bin\Package
"%zip%" a -tzip -r "%base_dir%\Package\%ZipFileName%" *
cd /d %current_dir%

echo Package %ZipFileName% created

goto :end


:EndErr
ENDLOCAL
echo Build failed
exit /B 1

:End
ENDLOCAL
exit /B 0
