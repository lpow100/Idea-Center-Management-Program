@echo off

echo Building project...

make
if %errorlevel% neq 0 (
    echo.
    echo [!] Build failed. check the errors above.
    pause
    exit /b %errorlevel%
)

echo.
echo Launching game.exe...
game.exe

pause