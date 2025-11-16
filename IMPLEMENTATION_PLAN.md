# AvsPmod Python 3.14 Migration - Implementation Plan

## Project Information
- **Repository:** https://github.com/senthilnayagam/AvsPmod-2025
- **Forked From:** https://github.com/AvsPmod/AvsPmod
- **Current Version:** 2.5.1 (2015)
- **Target Python:** 3.14
- **Current Python:** 2.6-2.7

---

## Phase 0: Preparation & Environment Setup

### 0.1 Create Development Environment
**Priority:** P0 | **Duration:** 1-2 hours

**Tasks:**
1. ✅ Fork repository (DONE)
2. ✅ Create local development branch: `python3-migration`
3. Install Python 3.14 (or latest 3.12+)
4. Set up virtual environment
5. Document current Python version baseline
6. Create backup of original codebase

**Commands:**
```bash
# Create virtual environment
python3.14 -m venv venv-py314
venv-py314\Scripts\activate

# Install initial dependencies
pip install --upgrade pip setuptools wheel
```

**Deliverables:**
- [ ] Working Python 3.14 environment
- [ ] Git branch: `python3-migration`
- [ ] Baseline documentation

---

### 0.2 Install Modern Dependencies
**Priority:** P0 | **Duration:** 2-3 hours

**Tasks:**
1. Create `requirements.txt` with Python 3 compatible versions
2. Attempt to install wxPython 4.x
3. Install development/testing tools
4. Document any installation issues

**Required Packages:**
```
wxPython>=4.2.0
cffi>=1.16.0
pycparser>=2.21
PyInstaller>=6.0
pytest>=7.0
black>=23.0
flake8>=6.0
```

**Commands:**
```bash
pip install wxPython
pip install -r requirements.txt
```

**Deliverables:**
- [ ] `requirements.txt` file created
- [ ] `requirements-dev.txt` for development tools
- [ ] All packages installed successfully
- [ ] Installation notes documented

---

### 0.3 Static Analysis & Documentation
**Priority:** P0 | **Duration:** 2-3 hours

**Tasks:**
1. Run `2to3` in preview mode (no changes)
2. Generate initial conversion report
3. Catalog all Python 2 specific patterns
4. Create file-by-file migration checklist
5. Identify all print statements
6. Identify all import statements to change

**Commands:**
```bash
# Run 2to3 analysis (don't apply changes yet)
2to3 --print-function *.py > migration-analysis.txt

# Find all Python 2 patterns
grep -r "print " --include="*.py" . > print-statements.txt
grep -r "import " --include="*.py" . > imports.txt
grep -r "except.*," --include="*.py" . > exceptions.txt
```

**Deliverables:**
- [ ] `migration-analysis.txt` - Full 2to3 output
- [ ] `MIGRATION_CHECKLIST.md` - Detailed task list
- [ ] Pattern catalog of all needed changes

---

## Phase 1: Automated Syntax Conversion

### 1.1 Run 2to3 Tool
**Priority:** P0 | **Duration:** 3-4 hours

**Tasks:**
1. Backup original files
2. Run `2to3` on all Python files
3. Review generated changes
4. Apply changes to test files first
5. Commit changes with clear messages

**Commands:**
```bash
# Backup
git add .
git commit -m "Checkpoint: Pre-2to3 conversion"

# Run 2to3 (start with single file)
2to3 -w global_vars.py

# Then expand to more files
2to3 -w i18n.py icons.py
2to3 -w avisynth.py avisynth_cffi.py
2to3 -w pyavs.py pyavs_avifile.py
2to3 -w wxp.py

# Finally the main file
2to3 -w avsp.py
```

**Expected Changes:**
- Print statements → print()
- except X, e → except X as e
- dict.iteritems() → dict.items()
- xrange() → range()
- Raw_input() → input()
- unicode() → str()

**Deliverables:**
- [ ] All files converted by 2to3
- [ ] Git commits per file group
- [ ] Conversion notes document

---

### 1.2 Apply Pyupgrade
**Priority:** P1 | **Duration:** 2-3 hours

**Tasks:**
1. Install pyupgrade
2. Run on all files to modernize syntax
3. Target Python 3.10+ idioms
4. Review and commit changes

**Commands:**
```bash
pip install pyupgrade

# Run on all Python files
pyupgrade --py310-plus *.py
pyupgrade --py310-plus macros/**/*.py
pyupgrade --py310-plus tools/*.py
```

**Deliverables:**
- [ ] Modernized Python 3 syntax
- [ ] F-strings where applicable
- [ ] Type hints added (optional)

---

### 1.3 Fix Import Statements Manually
**Priority:** P0 | **Duration:** 4-6 hours

**Tasks:**
1. Replace `import cPickle` → `import pickle`
2. Replace `import StringIO` → `from io import StringIO`
3. Replace `import urllib2` → `from urllib import request as urllib2`
4. Replace `import _winreg` → `import winreg`
5. Replace `import thread` → `import _thread as thread`
6. Update all usage of changed modules
7. Test each import change

**File Priority:**
1. avsp.py (main file)
2. wxp.py
3. avisynth.py
4. Other files

**Example Changes:**

```python
# OLD (Python 2)
import cPickle
data = cPickle.load(f)

# NEW (Python 3)
import pickle
data = pickle.load(f)
```

```python
# OLD (Python 2)
import StringIO
buf = StringIO.StringIO()

# NEW (Python 3)
from io import StringIO
buf = StringIO()
```

**Deliverables:**
- [ ] All imports updated to Python 3
- [ ] Code still syntactically valid
- [ ] Separate commits per import type

---

## Phase 2: wxPython Phoenix Migration

### 2.1 Audit wxPython Usage
**Priority:** P0 | **Duration:** 3-4 hours

**Tasks:**
1. List all wx.* usage in codebase
2. Cross-reference with Phoenix migration guide
3. Create compatibility shim layer (if needed)
4. Document breaking changes

**Commands:**
```bash
# Find all wxPython usage
grep -r "wx\." --include="*.py" . > wx-usage.txt
grep -r "wxButtons" --include="*.py" . >> wx-usage.txt
grep -r "stc\." --include="*.py" . >> wx-usage.txt
```

**Key Areas:**
- `wx.VERSION` checks
- `SetToolTipString()` calls
- `AddSimpleTool()` calls
- Event handlers
- AGW widget usage

**Deliverables:**
- [ ] Complete wx usage catalog
- [ ] Phoenix compatibility notes
- [ ] Migration strategy per issue

---

### 2.2 Update wxPython API Calls
**Priority:** P0 | **Duration:** 8-12 hours

**Tasks:**
1. Remove `wxversion.select()` calls
2. Update wx.VERSION checks
3. Update toolbar API calls
4. Update tooltip methods
5. Fix event handler signatures
6. Test each change incrementally

**Major Changes:**

```python
# OLD (Python 2 + wxPython 2.8)
import wxversion
wxversion.select('2.8')
import wx

if wx.VERSION < (2, 9):
    # ...

# NEW (Python 3 + wxPython 4.x)
import wx

wx_version = wx.version()  # Returns string like '4.2.0'
if wx.VERSION_STRING < '4.0':
    # ...
```

```python
# OLD
button.SetToolTipString("Help text")

# NEW
button.SetToolTip("Help text")
```

```python
# OLD
toolbar.AddSimpleTool(id, bitmap, "Label")

# NEW
toolbar.AddTool(id, "Label", bitmap)
```

**Deliverables:**
- [ ] All wx API calls updated
- [ ] Application starts without wx errors
- [ ] Basic UI renders correctly

---

### 2.3 Fix Styled Text Control (STC) Issues
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Review STC usage in avsp.py
2. Update deprecated STC constants
3. Fix style setting methods
4. Test text editor functionality

**Areas to Check:**
- Line number margins
- Syntax highlighting
- Auto-completion
- Call tips
- Markers and indicators

**Deliverables:**
- [ ] Text editor displays correctly
- [ ] Syntax highlighting works
- [ ] No STC-related errors

---

## Phase 3: Core Functionality Fixes

### 3.1 String/Bytes/Unicode Handling
**Priority:** P0 | **Duration:** 6-8 hours

**Tasks:**
1. Audit all file I/O operations
2. Add explicit encoding to all `open()` calls
3. Fix binary data handling (video frames)
4. Update string type checks
5. Test with non-ASCII filenames

**Key Changes:**

```python
# OLD
f = open(filename, 'r')

# NEW
f = open(filename, 'r', encoding='utf-8')
```

```python
# OLD
if isinstance(text, basestring):
    # ...

# NEW
if isinstance(text, str):
    # ...
```

```python
# OLD
if isinstance(path, unicode):
    path = path.encode('mbcs')

# NEW
if isinstance(path, str):
    path = path.encode('mbcs')
```

**Deliverables:**
- [ ] All file operations have encoding
- [ ] Binary data handled correctly
- [ ] Unicode filenames work

---

### 3.2 Fix Integer Division
**Priority:** P1 | **Duration:** 3-4 hours

**Tasks:**
1. Find all `/` operators in calculations
2. Determine if integer or float division intended
3. Update to `//` where needed (frame numbers!)
4. Test frame seeking accuracy

**Critical Areas:**
- Frame number calculations
- Timeline positioning
- Video dimensions
- Pixel coordinates

**Example:**
```python
# OLD (Python 2 - integer division)
frame = position / framerate

# NEW (Python 3 - need explicit integer division)
frame = position // framerate  # If integer needed
# OR
frame = int(position / framerate)
```

**Deliverables:**
- [ ] All division operators reviewed
- [ ] Frame calculations accurate
- [ ] No rounding errors in UI

---

### 3.3 Fix Dictionary Iteration
**Priority:** P1 | **Duration:** 2-3 hours

**Tasks:**
1. Replace `dict.iteritems()` → `dict.items()`
2. Replace `dict.iterkeys()` → `dict.keys()`
3. Replace `dict.itervalues()` → `dict.values()`
4. Update loop variables if needed

**Note:** 2to3 should catch most of these, but verify manually.

**Deliverables:**
- [ ] All dict iterations updated
- [ ] No performance regressions

---

## Phase 4: AviSynth Integration

### 4.1 Update avisynth.py (32-bit)
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Fix print statements
2. Update unicode checks
3. Test ctypes structures
4. Verify DLL loading
5. Test basic clip creation

**Testing:**
```python
import avisynth
env = avisynth.avs.AVS_ScriptEnvironment()
clip = env.Invoke('Version')
print(clip.Width, clip.Height)
```

**Deliverables:**
- [ ] avisynth.py works on 32-bit Python 3
- [ ] Can load AviSynth DLL
- [ ] Can create simple clips

---

### 4.2 Update avisynth_cffi.py (64-bit)
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Update cffi to 1.16+
2. Fix Python 3 compatibility
3. Update header file handling
4. Test 64-bit AviSynth+ loading
5. Verify frame data access

**Deliverables:**
- [ ] avisynth_cffi.py works on 64-bit Python 3
- [ ] Can load AviSynth+ 64-bit DLL
- [ ] Frame buffer access works

---

### 4.3 Update pyavs.py and pyavs_avifile.py
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Fix print statements
2. Update video frame handling
3. Test RGB/YUV conversion
4. Verify frame buffer operations
5. Test with actual video files

**Critical Functions:**
- `GetFrame()`
- `DrawFrame()`
- Color space conversions
- Buffer allocation

**Deliverables:**
- [ ] Video preview displays frames
- [ ] No frame data corruption
- [ ] Color conversion accurate

---

## Phase 5: Build System Migration

### 5.1 Create PyInstaller Spec File
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Generate initial spec file
2. Add all data files (help/, macros/, tools/)
3. Include icon file
4. Configure hidden imports
5. Test basic build

**Commands:**
```bash
# Generate spec file
pyi-makespec --name=AvsPmod --windowed --icon=AvsP.ico avsp.py

# Edit AvsPmod.spec to add data files
# Build
pyinstaller AvsPmod.spec
```

**Deliverables:**
- [ ] `AvsPmod.spec` file created
- [ ] All resources included
- [ ] Test build completes

---

### 5.2 Replace build.py
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Create new build.py for PyInstaller
2. Add version detection
3. Include UPX compression
4. Create ZIP/7z archive
5. Test full build process

**Deliverables:**
- [ ] New build.py script
- [ ] Automated build process
- [ ] Output executable works

---

### 5.3 Update setup.py
**Priority:** P2 | **Duration:** 2-3 hours

**Tasks:**
1. Remove py2exe code
2. Add setuptools configuration
3. Update metadata
4. Test `pip install -e .`

**Deliverables:**
- [ ] Modern setup.py
- [ ] Package installable via pip
- [ ] Development mode works

---

## Phase 6: Testing & Validation

### 6.1 Unit Testing Setup
**Priority:** P1 | **Duration:** 4-6 hours

**Tasks:**
1. Create `tests/` directory
2. Write basic import tests
3. Test each module in isolation
4. Create test fixtures for video files
5. Set up pytest configuration

**Test Areas:**
- Import all modules
- AviSynth environment creation
- Clip loading
- Frame retrieval
- File I/O operations

**Deliverables:**
- [ ] `tests/` directory structure
- [ ] Basic test suite
- [ ] All tests passing

---

### 6.2 Integration Testing
**Priority:** P0 | **Duration:** 8-12 hours

**Tasks:**
1. Test application startup
2. Test opening .avs files
3. Test syntax highlighting
4. Test video preview
5. Test all menu functions
6. Test keyboard shortcuts
7. Test user sliders
8. Test bookmarks
9. Test macros
10. Test crop editor

**Test Scenarios:**
- [ ] Launch application
- [ ] Create new script
- [ ] Open existing script
- [ ] Load video preview
- [ ] Navigate frames
- [ ] Save script
- [ ] Use slider controls
- [ ] Execute macro
- [ ] Use crop editor
- [ ] Export HTML

**Deliverables:**
- [ ] Integration test checklist
- [ ] All core features working
- [ ] Bug list for fixes

---

### 6.3 Macro Migration
**Priority:** P2 | **Duration:** 4-6 hours

**Tasks:**
1. Update all macro scripts to Python 3
2. Test each macro individually
3. Fix any macro API changes
4. Update macro documentation

**Macros to Update:**
- Bookmarks at Intervals.py
- Bookmarks to Chapter.py
- DeleteFrame.py
- DuplicateFrame.py
- Save Image Sequence.py
- All Example macros

**Deliverables:**
- [ ] All macros Python 3 compatible
- [ ] All macros tested
- [ ] Macro documentation updated

---

### 6.4 Tools Migration
**Priority:** P2 | **Duration:** 3-4 hours

**Tasks:**
1. Update tool scripts to Python 3
2. Test encoder GUI
3. Test resize calculator
4. Test avs2avi GUI

**Deliverables:**
- [ ] All tools working
- [ ] Tool integration tested

---

## Phase 7: Performance & Polish

### 7.1 Performance Profiling
**Priority:** P2 | **Duration:** 3-4 hours

**Tasks:**
1. Profile startup time
2. Profile frame rendering
3. Profile large script handling
4. Identify bottlenecks
5. Optimize critical paths

**Deliverables:**
- [ ] Performance baseline
- [ ] Optimization opportunities identified
- [ ] Critical issues fixed

---

### 7.2 Code Quality Improvements
**Priority:** P2 | **Duration:** 4-6 hours

**Tasks:**
1. Run black formatter
2. Run flake8 linter
3. Fix linting issues
4. Add type hints to critical functions
5. Update docstrings

**Commands:**
```bash
black *.py
flake8 *.py --max-line-length=100
```

**Deliverables:**
- [ ] Consistent code formatting
- [ ] No linting errors
- [ ] Improved code quality

---

### 7.3 Documentation Updates
**Priority:** P2 | **Duration:** 4-6 hours

**Tasks:**
1. Update README.md
2. Update build instructions
3. Update requirements
4. Create CHANGELOG entry
5. Update help files
6. Create migration notes

**Deliverables:**
- [ ] Updated README.md
- [ ] Updated documentation
- [ ] Migration guide created

---

## Phase 8: Release Preparation

### 8.1 Version Bump
**Priority:** P0 | **Duration:** 1 hour

**Tasks:**
1. Update version in global_vars.py
2. Update version in setup.py
3. Create changelog entry
4. Tag release in git

**Version Scheme:**
- Consider: 3.0.0 (major version for Python 3)
- Or: 2.6.0 (minor update from 2.5.1)

**Deliverables:**
- [ ] Version updated everywhere
- [ ] Git tag created

---

### 8.2 Build Release Packages
**Priority:** P0 | **Duration:** 2-3 hours

**Tasks:**
1. Build Windows 32-bit version
2. Build Windows 64-bit version
3. Test both executables
4. Create installer (optional)
5. Create ZIP archives

**Deliverables:**
- [ ] AvsPmod-win32.zip
- [ ] AvsPmod-win64.zip
- [ ] Both tested and working

---

### 8.3 Create GitHub Release
**Priority:** P0 | **Duration:** 1-2 hours

**Tasks:**
1. Draft release notes
2. Upload build artifacts
3. Document breaking changes
4. List Python 3.14 compatibility
5. Publish release

**Release Notes Template:**
```markdown
# AvsPmod 3.0.0 - Python 3.14 Compatible

## Major Changes
- Migrated to Python 3.14
- Updated to wxPython 4.2
- Replaced py2exe with PyInstaller

## Breaking Changes
- Requires Python 3.8+ (3.14 recommended)
- Requires AviSynth+ for 64-bit support
- Some old macros may need updates

## Installation
[Instructions]

## Known Issues
[List any remaining issues]
```

**Deliverables:**
- [ ] GitHub release created
- [ ] Release notes published
- [ ] Artifacts uploaded

---

## Rollback Plan

### If Migration Fails
1. Keep Python 2.7 version in separate branch
2. Document all attempted changes
3. Create compatibility layer instead
4. Consider partial migration (core only)

### Fallback Strategies
1. **Target Python 3.10 instead of 3.14** - Better library support
2. **Use compatibility shims** - Maintain Python 2/3 compatibility
3. **Freeze dependencies** - Use older but stable versions
4. **Split codebase** - Migrate incrementally

---

## Success Metrics

### Minimum Viable Product (MVP)
- ✅ Application launches
- ✅ Can edit scripts
- ✅ Syntax highlighting works
- ✅ Can save files

### Full Success
- ✅ Video preview works
- ✅ All menus functional
- ✅ Macros work
- ✅ Build creates executables
- ✅ Performance acceptable
- ✅ No Python 3 warnings

### Stretch Goals
- ✅ Modern Python 3 idioms (f-strings, type hints)
- ✅ Improved performance
- ✅ Better error handling
- ✅ Updated UI/UX

---

## Timeline Summary

| Phase | Duration | Cumulative |
|-------|----------|------------|
| 0. Preparation | 5-8 hours | 5-8h |
| 1. Syntax Conversion | 9-13 hours | 14-21h |
| 2. wxPython Migration | 15-22 hours | 29-43h |
| 3. Core Functionality | 11-15 hours | 40-58h |
| 4. AviSynth Integration | 12-18 hours | 52-76h |
| 5. Build System | 10-15 hours | 62-91h |
| 6. Testing | 19-28 hours | 81-119h |
| 7. Polish | 11-16 hours | 92-135h |
| 8. Release | 4-6 hours | 96-141h |

**Total Estimated Time:** 96-141 hours (2.5-3.5 weeks full-time)

---

## Risk Mitigation

### High-Risk Items
1. **wxPython Phoenix issues** → Allocate extra time, have fallback UI plans
2. **AviSynth ctypes compatibility** → Test early, may need cffi rewrite
3. **Performance regressions** → Profile early and often

### Contingency Buffer
- Add 20-30% time buffer for unexpected issues
- Plan for 2-3 "unknown unknowns"
- Keep communication open with original maintainers

---

## Next Steps

### Immediate Actions (Start Today)
1. ✅ Create `requirements.txt`
2. ✅ Set up Python 3.14 virtual environment
3. ✅ Run 2to3 analysis
4. ✅ Create git branch
5. Start Phase 1.1 (automated conversion)

### Week 1 Goals
- [ ] Complete Phases 0-1 (Preparation + Syntax)
- [ ] Start Phase 2 (wxPython)
- [ ] Have application that starts (even with errors)

### Week 2 Goals
- [ ] Complete Phase 2-3 (wxPython + Core)
- [ ] Start Phase 4 (AviSynth)
- [ ] Have basic functionality working

### Week 3 Goals
- [ ] Complete Phases 4-6 (AviSynth + Build + Testing)
- [ ] Start Phase 7 (Polish)
- [ ] Have all features working

### Week 4 Goals
- [ ] Complete Phases 7-8 (Polish + Release)
- [ ] Final testing
- [ ] Release version 3.0.0

---

## Support & Resources

### Documentation
- Python 2→3 Guide: https://docs.python.org/3/howto/pyporting.html
- wxPython Phoenix: https://wxpython.org/Phoenix/docs/html/
- PyInstaller: https://pyinstaller.org/en/stable/

### Tools
- 2to3: Built into Python
- pyupgrade: `pip install pyupgrade`
- black: `pip install black`
- flake8: `pip install flake8`

### Community
- wxPython Users Group: https://groups.google.com/g/wxpython-users
- Python Discord: https://discord.gg/python
- AviSynth Forum: https://forum.doom9.org/

---

**Plan Version:** 1.0  
**Last Updated:** 2025-11-16  
**Status:** READY TO START  
**Next Action:** Phase 0.1 - Create Development Environment
