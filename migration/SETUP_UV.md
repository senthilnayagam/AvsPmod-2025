# Setup Guide: Using UV for Python 3.14

This guide shows how to set up the AvsPmod development environment using `uv`, a fast Python package installer and environment manager.

## What is UV?

UV is a blazingly fast Python package installer and resolver written in Rust. It's 10-100x faster than pip and can manage Python versions too!

- GitHub: https://github.com/astral-sh/uv
- Docs: https://docs.astral.sh/uv/

## Step 1: Install UV

### Windows (PowerShell)
```powershell
# Using installer
powershell -c "irm https://astral.sh/uv/install.ps1 | iex"

# Or using pip
pip install uv
```

### Verify Installation
```powershell
uv --version
```

## Step 2: Install Python 3.14 with UV

UV can install and manage Python versions for you!

```powershell
# Install Python 3.14 (or latest 3.12+ if 3.14 not available yet)
uv python install 3.14

# Or install latest Python 3.12 (stable)
uv python install 3.12

# List available Python versions
uv python list

# List installed Python versions
uv python list --only-installed
```

## Step 3: Create Project Virtual Environment

Navigate to the project directory:

```powershell
cd C:\Users\senth\code\AvsPmod-2025
```

Create a virtual environment with Python 3.14:

```powershell
# Create venv with Python 3.14
uv venv --python 3.14

# Or with Python 3.12 if 3.14 not available
uv venv --python 3.12

# Or let UV choose the best available version
uv venv
```

This creates a `.venv` directory in your project.

## Step 4: Activate the Virtual Environment

```powershell
# Windows PowerShell
.\.venv\Scripts\Activate.ps1

# Windows CMD
.\.venv\Scripts\activate.bat
```

You should see `(.venv)` in your prompt.

## Step 5: Install Requirements with UV

UV is much faster than pip for installing packages!

```powershell
# Install all requirements
uv pip install -r requirements.txt

# Or compile dependencies and install (recommended for reproducible builds)
uv pip compile requirements.txt -o requirements.lock
uv pip sync requirements.lock
```

### Install Development Dependencies

```powershell
# Create requirements-dev.txt if it doesn't exist
# Then install
uv pip install -r requirements-dev.txt
```

## Step 6: Verify Installation

```powershell
# Check Python version
python --version

# Check installed packages
uv pip list

# Test imports
python -c "import sys; print(f'Python {sys.version}')"
python -c "import wx; print(f'wxPython {wx.version()}')"
```

## Complete Setup Script (PowerShell)

Here's a complete script to set everything up:

```powershell
# AvsPmod Python 3.14 Setup Script (using UV)

Write-Host "=== AvsPmod Python 3.14 Migration Setup ===" -ForegroundColor Green

# Step 1: Check if UV is installed
Write-Host "`n[1/6] Checking UV installation..." -ForegroundColor Yellow
if (!(Get-Command uv -ErrorAction SilentlyContinue)) {
    Write-Host "UV not found. Installing UV..." -ForegroundColor Cyan
    irm https://astral.sh/uv/install.ps1 | iex
    # Refresh PATH
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
}
uv --version

# Step 2: Install Python 3.14 (or 3.12)
Write-Host "`n[2/6] Installing Python 3.14..." -ForegroundColor Yellow
uv python install 3.14 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Host "Python 3.14 not available, installing Python 3.12..." -ForegroundColor Cyan
    uv python install 3.12
}

# Step 3: Create virtual environment
Write-Host "`n[3/6] Creating virtual environment..." -ForegroundColor Yellow
if (Test-Path .venv) {
    Write-Host "Virtual environment already exists, recreating..." -ForegroundColor Cyan
    Remove-Item -Recurse -Force .venv
}
uv venv --python 3.14 2>$null
if ($LASTEXITCODE -ne 0) {
    uv venv --python 3.12
}

# Step 4: Activate virtual environment
Write-Host "`n[4/6] Activating virtual environment..." -ForegroundColor Yellow
& .\.venv\Scripts\Activate.ps1

# Step 5: Install requirements
Write-Host "`n[5/6] Installing dependencies..." -ForegroundColor Yellow
uv pip install -r requirements.txt

# Step 6: Verify installation
Write-Host "`n[6/6] Verifying installation..." -ForegroundColor Yellow
python --version
Write-Host ""
uv pip list | Select-String -Pattern "wxPython|cffi|PyInstaller" -Context 0,0

Write-Host "`n=== Setup Complete! ===" -ForegroundColor Green
Write-Host "`nNext steps:" -ForegroundColor Cyan
Write-Host "  1. Test imports: python -c 'import wx; print(wx.version())'" 
Write-Host "  2. Read IMPLEMENTATION_PLAN.md for migration steps"
Write-Host "  3. Start Phase 2: Update wxPython API calls"
```

Save this as `setup-uv.ps1` and run:

```powershell
.\setup-uv.ps1
```

## Alternative: Manual Step-by-Step

If you prefer to run commands manually:

```powershell
# Install UV
irm https://astral.sh/uv/install.ps1 | iex

# Install Python
uv python install 3.14

# Create venv
uv venv --python 3.14

# Activate
.\.venv\Scripts\Activate.ps1

# Install packages
uv pip install -r requirements.txt

# Verify
python -c "import wx; print('wxPython OK:', wx.version())"
python -c "import global_vars; print('AvsPmod version:', global_vars.version)"
```

## UV Advantages Over pip

- ⚡ **10-100x faster** than pip
- 🔒 **Better dependency resolution** 
- 🐍 **Python version management** built-in
- 📦 **Lock files** for reproducible builds
- 💾 **Disk space efficient** (shared cache)
- 🔄 **Compatible with pip** (same commands)

## Troubleshooting

### Issue: UV not found after installation
```powershell
# Refresh PATH
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Or restart PowerShell
```

### Issue: Python 3.14 not available
```powershell
# Use Python 3.12 instead (fully compatible)
uv python install 3.12
uv venv --python 3.12
```

### Issue: wxPython installation fails
```powershell
# wxPython may need manual download on some systems
# Download wheel from: https://extras.wxpython.org/wxPython4/extras/
# Then install: uv pip install path/to/wxPython.whl

# Or use pip for just wxPython
pip install wxPython
uv pip install -r requirements.txt --exclude wxPython
```

### Issue: Permission denied
```powershell
# Run PowerShell as Administrator
# Or use --user flag
uv pip install -r requirements.txt --user
```

## UV Commands Cheat Sheet

```powershell
# Python management
uv python list                    # List available Python versions
uv python list --only-installed   # List installed versions
uv python install 3.14            # Install Python 3.14
uv python uninstall 3.14          # Uninstall Python 3.14

# Virtual environments
uv venv                           # Create venv with default Python
uv venv --python 3.14             # Create venv with Python 3.14
uv venv .venv                     # Create venv in specific directory

# Package management
uv pip install package            # Install package
uv pip install -r requirements.txt # Install from requirements
uv pip list                       # List installed packages
uv pip freeze                     # Show installed packages (pip format)
uv pip uninstall package          # Uninstall package

# Lock files (recommended)
uv pip compile requirements.txt   # Create requirements.lock
uv pip sync requirements.lock     # Install from lock file
```

## Next Steps After Setup

Once your environment is ready:

1. **Test basic imports:**
   ```powershell
   python -c "import global_vars; print('OK')"
   python -c "import i18n; print('OK')"
   ```

2. **Test wxPython:**
   ```powershell
   python -c "import wx; print(wx.version())"
   ```

3. **Try running the app** (will likely fail, that's expected):
   ```powershell
   python avsp.py
   ```

4. **Continue with Phase 2** - wxPython API migration
   - See `IMPLEMENTATION_PLAN.md` for details

## Resources

- UV Documentation: https://docs.astral.sh/uv/
- UV GitHub: https://github.com/astral-sh/uv
- wxPython Downloads: https://wxpython.org/pages/downloads/
- Python 3.14 What's New: https://docs.python.org/3.14/whatsnew/

---

**Created:** 2025-11-16  
**For:** AvsPmod Python 3.14 Migration  
**Status:** Ready to use
