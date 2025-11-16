# Quick Reference - AvsPmod Python 3.14 Migration

## One-Command Setup (Recommended)

```powershell
# PowerShell (Automated - Installs everything)
.\setup-uv.ps1
```

## Manual Setup Commands

### Option 1: Using UV (Fast! ⚡)
```powershell
# Install UV
irm https://astral.sh/uv/install.ps1 | iex

# Install Python 3.14
uv python install 3.14

# Create & activate venv
uv venv --python 3.14
.\.venv\Scripts\Activate.ps1

# Install dependencies
uv pip install -r requirements.txt
```

### Option 2: Traditional pip
```powershell
# Create & activate venv
python -m venv venv-py314
.\venv-py314\Scripts\activate

# Install dependencies
pip install -r requirements.txt
```

## Common Tasks

### Activate Environment
```powershell
# UV venv
.\.venv\Scripts\Activate.ps1

# Traditional venv
.\venv-py314\Scripts\activate
```

### Test Installation
```powershell
# Check Python version
python --version

# Test imports
python -c "import global_vars; print('AvsPmod:', global_vars.version)"
python -c "import wx; print('wxPython:', wx.version())"
```

### Run Application
```powershell
# Try launching (may fail - that's expected in migration)
python avsp.py
```

### Install Additional Packages
```powershell
# With UV (fast)
uv pip install package_name

# With pip
pip install package_name
```

## Git Workflow

### Check Current Status
```powershell
git status
git branch
git log --oneline -5
```

### Commit Changes
```powershell
git add .
git commit -m "Description of changes"
```

### View Changes
```powershell
git diff
git diff --cached  # Staged changes
```

## Migration Phases

- ✅ Phase 0: Setup (DONE)
- ✅ Phase 1: Syntax Conversion (DONE)  
- ⏳ Phase 2: wxPython API (IN PROGRESS)
- ⏭️ Phase 3: String/Bytes Handling
- ⏭️ Phase 4: AviSynth Integration
- ⏭️ Phase 5: Build System
- ⏭️ Phase 6: Testing

## Key Files

| File | Purpose |
|------|---------|
| `setup-uv.ps1` | Automated setup script |
| `SETUP_UV.md` | Detailed UV setup guide |
| `IMPLEMENTATION_PLAN.md` | Full migration plan |
| `MIGRATION_CHECKLIST.md` | Task checklist |
| `PROGRESS_REPORT.md` | Current progress |
| `requirements.txt` | Python dependencies |
| `avsp.py` | Main application file |

## Useful Python Commands

```powershell
# Test a specific module
python -c "import module_name; print('OK')"

# Check syntax without running
python -m py_compile filename.py

# Format code
black filename.py

# Lint code
flake8 filename.py

# Run tests (when available)
pytest tests/
```

## Documentation

- **Getting Started:** README_MIGRATION.md
- **UV Setup:** SETUP_UV.md  
- **Detailed Plan:** IMPLEMENTATION_PLAN.md
- **Task List:** MIGRATION_CHECKLIST.md
- **Progress:** PROGRESS_REPORT.md
- **Technical Analysis:** agents.md

## Troubleshooting

### Virtual Environment Issues
```powershell
# Deactivate current environment
deactivate

# Remove and recreate
Remove-Item -Recurse -Force .venv
uv venv --python 3.14
```

### Import Errors
```powershell
# Verify package is installed
uv pip list | Select-String "package_name"

# Reinstall specific package
uv pip uninstall package_name
uv pip install package_name
```

### wxPython Issues
```powershell
# wxPython may need manual installation
# Download from: https://extras.wxpython.org/wxPython4/extras/
# Then: uv pip install path/to/wxPython.whl
```

## Next Steps

1. **Ensure environment is set up:** Run `.\setup-uv.ps1`
2. **Test basic imports:** See commands above
3. **Read the plan:** `IMPLEMENTATION_PLAN.md`
4. **Start Phase 2:** wxPython API migration

## Help & Resources

- UV Docs: https://docs.astral.sh/uv/
- wxPython: https://wxpython.org/
- Python 3 Migration: https://docs.python.org/3/howto/pyporting.html
- Project Issues: Check PROGRESS_REPORT.md

---

**Quick Tip:** Use `uv` instead of `pip` for 10-100x faster package installation!

**Last Updated:** 2025-11-16
