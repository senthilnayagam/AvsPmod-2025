@echo off
REM AvsPmod Python 3.14 Setup Script (using UV)
REM Run this in Command Prompt

echo === AvsPmod Python 3.14 Migration Setup ===
echo.

REM Check if UV is installed
echo [1/6] Checking UV installation...
where uv >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo   UV not found. Please install UV first:
    echo   Run in PowerShell: irm https://astral.sh/uv/install.ps1 ^| iex
    echo   Or visit: https://docs.astral.sh/uv/
    pause
    exit /b 1
)
uv --version
echo   UV is installed!

REM Install Python
echo.
echo [2/6] Installing Python 3.14...
uv python install 3.14 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo   Python 3.14 not available, installing 3.12...
    uv python install 3.12
)
echo   Python installed!

REM Create virtual environment
echo.
echo [3/6] Creating virtual environment...
if exist .venv (
    echo   Virtual environment already exists. Delete .venv to recreate.
) else (
    uv venv
    echo   Virtual environment created!
)

REM Activate virtual environment
echo.
echo [4/6] Activating virtual environment...
call .venv\Scripts\activate.bat
echo   Virtual environment activated!

REM Install requirements
echo.
echo [5/6] Installing dependencies...
uv pip install -r requirements.txt
echo   Dependencies installed!

REM Verify installation
echo.
echo [6/6] Verifying installation...
python --version
echo.
uv pip list | findstr "wxPython cffi PyInstaller"

echo.
echo === Setup Complete! ===
echo.
echo Next steps:
echo   1. Activate virtual environment: .venv\Scripts\activate.bat
echo   2. Test imports: python -c "import global_vars; print(global_vars.version)"
echo   3. Read IMPLEMENTATION_PLAN.md for migration details
echo   4. Continue with Phase 2 (wxPython API migration)
echo.
pause
