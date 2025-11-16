# AvsPmod Python 3.14 Setup Script (using UV)
# Run this script in PowerShell to set up the development environment

Write-Host "=== AvsPmod Python 3.14 Migration Setup ===" -ForegroundColor Green
Write-Host "This script will set up Python 3.14 and install all dependencies using UV`n" -ForegroundColor Cyan

# Step 1: Check if UV is installed
Write-Host "[1/6] Checking UV installation..." -ForegroundColor Yellow
if (!(Get-Command uv -ErrorAction SilentlyContinue)) {
    Write-Host "  UV not found. Installing UV..." -ForegroundColor Cyan
    try {
        irm https://astral.sh/uv/install.ps1 | iex
        # Refresh PATH
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
        Write-Host "  UV installed successfully!" -ForegroundColor Green
    } catch {
        Write-Host "  Failed to install UV. Please install manually from: https://docs.astral.sh/uv/" -ForegroundColor Red
        exit 1
    }
}
$uvVersion = uv --version
Write-Host "  UV version: $uvVersion" -ForegroundColor Green

# Step 2: Install Python 3.14 (or 3.12 fallback)
Write-Host "`n[2/6] Installing Python..." -ForegroundColor Yellow
$pythonVersion = "3.14"

Write-Host "  Attempting to install Python $pythonVersion..." -ForegroundColor Cyan
uv python install $pythonVersion 2>$null

if ($LASTEXITCODE -ne 0) {
    Write-Host "  Python $pythonVersion not available yet, trying 3.12..." -ForegroundColor Yellow
    $pythonVersion = "3.12"
    uv python install $pythonVersion
    if ($LASTEXITCODE -ne 0) {
        Write-Host "  Failed to install Python. Please check UV installation." -ForegroundColor Red
        exit 1
    }
}

Write-Host "  Python $pythonVersion installed successfully!" -ForegroundColor Green

# Show installed Python versions
Write-Host "  Installed Python versions:" -ForegroundColor Cyan
uv python list --only-installed

# Step 3: Create virtual environment
Write-Host "`n[3/6] Creating virtual environment..." -ForegroundColor Yellow

if (Test-Path .venv) {
    Write-Host "  Virtual environment already exists." -ForegroundColor Yellow
    $response = Read-Host "  Do you want to recreate it? (y/N)"
    if ($response -eq 'y' -or $response -eq 'Y') {
        Write-Host "  Removing old virtual environment..." -ForegroundColor Cyan
        Remove-Item -Recurse -Force .venv
        Write-Host "  Creating new virtual environment with Python $pythonVersion..." -ForegroundColor Cyan
        uv venv --python $pythonVersion
    } else {
        Write-Host "  Using existing virtual environment." -ForegroundColor Green
    }
} else {
    Write-Host "  Creating new virtual environment with Python $pythonVersion..." -ForegroundColor Cyan
    uv venv --python $pythonVersion
}

if (!(Test-Path .venv)) {
    Write-Host "  Failed to create virtual environment!" -ForegroundColor Red
    exit 1
}

Write-Host "  Virtual environment created in .venv" -ForegroundColor Green

# Step 4: Activate virtual environment
Write-Host "`n[4/6] Activating virtual environment..." -ForegroundColor Yellow

if (Test-Path .\.venv\Scripts\Activate.ps1) {
    & .\.venv\Scripts\Activate.ps1
    Write-Host "  Virtual environment activated!" -ForegroundColor Green
} else {
    Write-Host "  Failed to find activation script!" -ForegroundColor Red
    exit 1
}

# Step 5: Install requirements
Write-Host "`n[5/6] Installing dependencies..." -ForegroundColor Yellow

if (!(Test-Path requirements.txt)) {
    Write-Host "  requirements.txt not found!" -ForegroundColor Red
    exit 1
}

Write-Host "  Installing packages with UV (this may take a few minutes)..." -ForegroundColor Cyan
uv pip install -r requirements.txt

if ($LASTEXITCODE -ne 0) {
    Write-Host "  Some packages failed to install. Trying with pip as fallback..." -ForegroundColor Yellow
    pip install -r requirements.txt
}

Write-Host "  Dependencies installed!" -ForegroundColor Green

# Step 6: Verify installation
Write-Host "`n[6/6] Verifying installation..." -ForegroundColor Yellow

Write-Host "  Python version:" -ForegroundColor Cyan
python --version

Write-Host "`n  Key packages installed:" -ForegroundColor Cyan
uv pip list | Select-String -Pattern "wxPython|cffi|PyInstaller|pytest|black|flake8" -Context 0,0

Write-Host "`n  Testing imports..." -ForegroundColor Cyan
$testsPassed = 0
$totalTests = 3

# Test 1: Basic Python
python -c "import sys; print('  ✓ Python', sys.version.split()[0])" 2>$null
if ($LASTEXITCODE -eq 0) { $testsPassed++ }

# Test 2: AvsPmod modules
python -c "import global_vars; print('  ✓ global_vars (version', global_vars.version + ')')" 2>$null
if ($LASTEXITCODE -eq 0) { $testsPassed++ }

# Test 3: wxPython
python -c "import wx; print('  ✓ wxPython', wx.version())" 2>$null
if ($LASTEXITCODE -eq 0) { $testsPassed++ } else { Write-Host "  ✗ wxPython import failed (may need manual installation)" -ForegroundColor Yellow }

Write-Host "`n=== Setup Complete! ($testsPassed/$totalTests tests passed) ===" -ForegroundColor Green

if ($testsPassed -eq $totalTests) {
    Write-Host "`nEnvironment is ready! 🎉" -ForegroundColor Green
} else {
    Write-Host "`nEnvironment setup complete, but some issues detected." -ForegroundColor Yellow
    Write-Host "Check the output above for details." -ForegroundColor Yellow
}

Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "  1. Make sure you're in the virtual environment:" -ForegroundColor White
Write-Host "     .\.venv\Scripts\Activate.ps1" -ForegroundColor Gray
Write-Host "`n  2. Test basic functionality:" -ForegroundColor White  
Write-Host "     python -c 'import global_vars; print(global_vars.version)'" -ForegroundColor Gray
Write-Host "`n  3. Read the migration plan:" -ForegroundColor White
Write-Host "     Get-Content IMPLEMENTATION_PLAN.md" -ForegroundColor Gray
Write-Host "`n  4. Continue with Phase 2 (wxPython API migration)" -ForegroundColor White
Write-Host "     See IMPLEMENTATION_PLAN.md for details" -ForegroundColor Gray

Write-Host "`nFor more information, see:" -ForegroundColor Cyan
Write-Host "  - SETUP_UV.md (UV setup guide)"
Write-Host "  - README_MIGRATION.md (Migration overview)"
Write-Host "  - IMPLEMENTATION_PLAN.md (Detailed plan)"

Write-Host ""
