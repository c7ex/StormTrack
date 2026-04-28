@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

set OUTPUT_FILE=__listing.txt
set CURRENT_DIR=%CD%

if exist %OUTPUT_FILE% del %OUTPUT_FILE%

echo Building listing from .cpp and .hpp files recursively...
echo.

set COUNT=0

for /r %%F in (*.cpp *.hpp) do (
    set /a COUNT+=1
    
    set "FULL_PATH=%%F"
    
    set "REL_PATH=!FULL_PATH:%CURRENT_DIR%\=!"
    
    if "!REL_PATH!"=="!FULL_PATH!" set "REL_PATH=%%~nxF"
    
    echo [!COUNT!] Processing: !REL_PATH!
    
    echo =============================================================================== >> %OUTPUT_FILE%
    echo File: !REL_PATH! >> %OUTPUT_FILE%
    echo =============================================================================== >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
    
    type "%%F" >> %OUTPUT_FILE%
    
    echo. >> %OUTPUT_FILE%
    echo. >> %OUTPUT_FILE%
)

echo.
echo Done! Processed files: !COUNT!
echo Result saved to %OUTPUT_FILE%

pause