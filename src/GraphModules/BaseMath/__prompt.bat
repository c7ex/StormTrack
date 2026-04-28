@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

:: Set output file name
set OUTPUT_FILE=__listing.txt

:: Check if output file exists and delete it
if exist %OUTPUT_FILE% del %OUTPUT_FILE%

echo Building listing from .cpp and .hpp files...
echo.

:: File counter
set COUNT=0

:: Process all .cpp and .hpp files in current folder
for %%F in (*.cpp *.hpp) do (
    set /a COUNT+=1
    echo [!COUNT!] Processing: %%F
    
    :: Write header with filename
    echo =============================================================================== >> %OUTPUT_FILE%
    echo File: %%F >> %OUTPUT_FILE%
    echo =============================================================================== >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
    
    :: Write file content
    type "%%F" >> %OUTPUT_FILE%
    
    :: Add separator between files
    echo. >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
)

echo.
echo Done! Processed files: !COUNT!
echo Result saved to %OUTPUT_FILE%

pause