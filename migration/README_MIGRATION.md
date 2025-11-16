# AvsPmod Python 3.14 Migration - Quick Start Guide

## What is This Project?

**AvsPmod** is an advanced text editor for creating AviSynth video processing scripts. It was last updated in 2015 and runs on Python 2.6-2.7. This migration project updates it to run on modern Python 3.14.

## Current Status

- **Original Version:** 2.5.1 (2015)
- **Original Python:** 2.6-2.7
- **Target Python:** 3.14
- **Migration Status:** Planning Complete, Ready to Start

## What Has Been Done

✅ **Analysis Phase Complete**
- Reviewed entire codebase (~19,000 lines)
- Identified all Python 2 incompatibilities
- Documented all required changes
- Created comprehensive migration strategy

✅ **Documentation Created**
1. **agents.md** - Detailed technical analysis and agent specialization guide
2. **IMPLEMENTATION_PLAN.md** - Step-by-step migration plan with timeline
3. **MIGRATION_CHECKLIST.md** - Task-by-task checklist (200+ items)
4. **requirements.txt** - Python 3 dependencies
5. **README_MIGRATION.md** (this file) - Quick start guide

## Key Migration Challenges

### 1. Python 2 → 3 Syntax (CRITICAL)
- **Print statements:** `print "x"` → `print("x")`
- **Exception handling:** `except X, e:` → `except X as e:`
- **Imports:** cPickle, StringIO, urllib2, _winreg all changed
- **Unicode:** Everything is Unicode in Python 3
- **Division:** `/` now always returns float

### 2. wxPython 2.8 → 4.2 (CRITICAL)
- Complete UI framework upgrade
- Many API methods renamed or removed
- Different event handling patterns
- Migration guide: https://wxpython.org/Phoenix/docs/html/MigrationGuide.html

### 3. AviSynth Integration (HIGH RISK)
- ctypes bindings may break
- Binary data handling changed
- Need to test with both 32-bit and 64-bit AviSynth

### 4. Build System (REQUIRED)
- py2exe → PyInstaller
- Complete rewrite of build scripts

## How to Get Started

### Prerequisites
```bash
# Install Python 3.14 (or 3.12+)
python --version  # Should show 3.14.x or 3.12+

# Install Git
git --version
```

### Quick Start with UV (Recommended - Fast!)

**UV is a blazingly fast Python package installer (10-100x faster than pip)**

```bash
# 1. Install UV
# PowerShell:
irm https://astral.sh/uv/install.ps1 | iex

# 2. Run the automated setup script
.\setup-uv.ps1

# That's it! The script will:
# - Install Python 3.14 (or 3.12)
# - Create virtual environment
# - Install all dependencies
# - Verify everything works
```

**See [SETUP_UV.md](SETUP_UV.md) for detailed UV setup instructions**

### Quick Start (Traditional Method)

```bash
# 1. Navigate to project
cd c:\Users\senth\code\AvsPmod-2025

# 2. Create virtual environment (traditional)
python -m venv venv-py314

# 3. Activate it
.\venv-py314\Scripts\activate

# 4. Install dependencies
pip install --upgrade pip setuptools wheel
pip install -r requirements.txt

# 5. Create migration branch
git checkout -b python3-migration
```

### Verify Installation

```bash
# Run 2to3 analysis (don't apply yet)
2to3 global_vars.py > analysis.txt
type analysis.txt

# Try importing a simple module
python -c "import global_vars; print(global_vars.version)"
# This will fail - that's expected!
```

## Migration Approach

### Phase 1: Core Files (Week 1)
1. Run automated tools (2to3, pyupgrade)
2. Fix imports manually
3. Get application to import without errors

### Phase 2: UI Working (Week 2)
1. Update wxPython API calls
2. Get window to display
3. Get text editor working

### Phase 3: Video Preview (Week 3)
1. Fix AviSynth bindings
2. Get video preview working
3. Test all features

### Phase 4: Release (Week 4)
1. Set up PyInstaller
2. Test thoroughly
3. Create release builds

## File Priority Order

Work on files in this order:

1. ✅ **global_vars.py** (43 lines) - Start here, easiest
2. **i18n.py** (457 lines) - Translations
3. **icons.py** - Should be compatible
4. **wxp.py** (2,289 lines) - UI framework
5. **avisynth.py** (1,254 lines) - AviSynth bindings
6. **pyavs.py** (880 lines) - Video handling
7. **avsp.py** (18,906 lines) - Main application (LAST!)

## Testing Strategy

### Incremental Testing
After each file conversion:
```bash
# Try importing the module
python -c "import module_name"

# Check for syntax errors
python -m py_compile module_name.py

# Run type checker
mypy module_name.py
```

### Integration Testing
Once main files work:
```bash
# Try launching (will probably fail at first)
python avsp.py

# Check what fails
python -m pdb avsp.py
```

## Common Issues & Solutions

### Issue: "No module named 'cPickle'"
**Solution:** Change `import cPickle` to `import pickle`

### Issue: "No module named 'StringIO'"
**Solution:** Change `import StringIO` to `from io import StringIO`

### Issue: "'str' object has no attribute 'decode'"
**Solution:** In Python 3, strings are already Unicode. Remove `.decode()` calls.

### Issue: wxPython import fails
**Solution:** 
```bash
pip install wxPython
# If that fails on Windows:
pip install -U wxPython
```

### Issue: "SyntaxError: invalid syntax" on print
**Solution:** Change `print "text"` to `print("text")`

## Useful Commands

```bash
# Find all print statements
grep -r "print " --include="*.py" .

# Find all cPickle imports
grep -r "cPickle" --include="*.py" .

# Find all exception handlers
grep -r "except.*," --include="*.py" .

# Run 2to3 on a file
2to3 -w filename.py

# Format code
black filename.py

# Check code style
flake8 filename.py
```

## Resources

### Documentation
- **This Project:**
  - `agents.md` - Technical analysis
  - `IMPLEMENTATION_PLAN.md` - Detailed plan
  - `MIGRATION_CHECKLIST.md` - Task list
  
- **Python:**
  - [Python 2 to 3 Guide](https://docs.python.org/3/howto/pyporting.html)
  - [What's New in Python 3](https://docs.python.org/3/whatsnew/)
  
- **wxPython:**
  - [Phoenix Migration Guide](https://wxpython.org/Phoenix/docs/html/MigrationGuide.html)
  - [Phoenix Documentation](https://wxpython.org/Phoenix/docs/html/)
  
- **Tools:**
  - [2to3 Documentation](https://docs.python.org/3/library/2to3.html)
  - [PyInstaller Manual](https://pyinstaller.org/)

### Getting Help
- wxPython Users: https://groups.google.com/g/wxpython-users
- Python Discord: https://discord.gg/python
- AviSynth Forum: https://forum.doom9.org/

## Expected Timeline

| Week | Goal | Key Deliverables |
|------|------|------------------|
| 1 | Basic compatibility | All files import without errors |
| 2 | UI working | Application window displays |
| 3 | Full functionality | Video preview works |
| 4 | Release ready | Builds and distributes |

**Total Estimated Time:** 96-141 hours (2.5-3.5 weeks full-time)

## Success Criteria

### Minimum (MVP)
- ✅ Application launches
- ✅ Can edit .avs files
- ✅ Syntax highlighting works
- ✅ Can save files

### Complete
- ✅ Video preview displays
- ✅ All menus work
- ✅ Macros execute
- ✅ Build creates .exe

## Next Actions

### Right Now
1. Read `IMPLEMENTATION_PLAN.md` for full details
2. Set up Python 3.14 environment
3. Install dependencies from `requirements.txt`
4. Start with Phase 0.2 (dependencies)

### This Week
1. Complete Phase 0 (Preparation)
2. Complete Phase 1 (Syntax conversion)
3. Start Phase 2 (wxPython)

### This Month
1. Complete all 8 phases
2. Release version 3.0.0
3. Celebrate! 🎉

## Questions?

- Check `agents.md` for technical deep-dive
- Check `IMPLEMENTATION_PLAN.md` for step-by-step guidance
- Check `MIGRATION_CHECKLIST.md` for task tracking

## Project Structure

```
AvsPmod-2025/
├── agents.md                    # Technical analysis (NEW)
├── IMPLEMENTATION_PLAN.md       # Detailed plan (NEW)
├── MIGRATION_CHECKLIST.md       # Task list (NEW)
├── README_MIGRATION.md          # This file (NEW)
├── requirements.txt             # Python 3 deps (NEW)
├── avsp.py                      # Main application
├── wxp.py                       # UI framework
├── avisynth.py                  # AviSynth bindings
├── pyavs.py                     # Video handling
├── global_vars.py               # Configuration
├── setup.py                     # Build script
├── macros/                      # User macros
├── tools/                       # Utilities
└── help/                        # Documentation
```

## Important Notes

⚠️ **Do Not Delete Python 2 Code Yet**
- Keep original code until migration complete
- Use git branches liberally
- Test thoroughly before removing old code

⚠️ **AviSynth Must Be Installed**
- AvsPmod requires AviSynth or AviSynth+
- Download from: http://avisynth.nl/
- 64-bit Python needs AviSynth+ 64-bit

⚠️ **Windows Only (For Now)**
- Original code supports Linux via AvxSynth
- Focus on Windows first
- Linux support can be added later

## Motivation

Why migrate to Python 3.14?
- Python 2 reached end-of-life in 2020
- Modern libraries require Python 3
- Better performance and security
- Active development and support
- Future-proof the application

## License

Same as original AvsPmod: GNU GPL v2

---

**Created:** 2025-11-16  
**Status:** Ready to Start  
**Next Step:** Phase 0.2 - Install Dependencies  
**Good Luck!** 🚀
