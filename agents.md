# AvsPmod Python 3.14 Migration - Agent Analysis

## Project Overview
**Project:** AvsPmod (AviSynth Script Editor)  
**Original Python Version:** 2.6-2.7  
**Target Python Version:** 3.14  
**Last Release:** 2015 (Version 2.5.1)  
**Repository:** https://github.com/AvsPmod/AvsPmod (forked)

## Executive Summary
AvsPmod is a tabbed text editor for creating AviSynth scripts with integrated video preview, syntax highlighting, and macro support. The codebase (~23,000 lines) was written for Python 2.6-2.7 and wxPython 2.8-2.9. Migrating to Python 3.14 requires addressing fundamental incompatibilities in syntax, standard library modules, and dependencies.

---

## Core Technologies & Dependencies

### Current Dependencies
- **Python:** 2.6-2.7 ❌
- **wxPython:** 2.8-2.9 ❌ (needs wxPython 4.x for Python 3)
- **py2exe:** 0.6.9 (build tool, Windows only) ❌
- **cffi:** 0.9.2 (for x64 AviSynth) ⚠️ (needs update)
- **AviSynth/AvxSynth:** Video processing library ✓ (external, should work)

### Target Dependencies
- **Python:** 3.14 ✓
- **wxPython:** 4.2.x (Phoenix) - major API changes
- **PyInstaller:** (replace py2exe)
- **cffi:** 1.16+ (modern version)

---

## Critical Migration Issues

### 1. Python 2 → 3 Syntax Changes (HIGH PRIORITY)

#### Import Statements
| Python 2 Module | Python 3 Equivalent | Files Affected |
|----------------|---------------------|----------------|
| `cPickle` | `pickle` | avsp.py, wxp.py |
| `StringIO` | `io.StringIO` | avsp.py, wxp.py |
| `urllib2` | `urllib.request` | avsp.py |
| `_winreg` | `winreg` | avsp.py |
| `thread` | `_thread` | avsp.py |

#### Print Statements
- All `print "text"` → `print("text")`
- Found in: avsp.py, pyavs.py, pyavs_avifile.py, avisynth.py

#### Exception Handling
- `except Exception, e:` → `except Exception as e:`
- Found throughout codebase

#### Integer Division
- `/` behavior changed (now always float division)
- `//` for integer division
- Risk in frame calculations

#### String/Unicode Handling
- All strings are Unicode in Python 3
- `unicode()` type removed
- `str/bytes` distinction critical for binary data

### 2. wxPython 2.x → 4.x (Phoenix) Migration (HIGH PRIORITY)

#### Major API Changes
- `wx.VERSION` tuple → `wx.version()` string
- `SetToolTipString()` → `SetToolTip()`
- `AddSimpleTool()` → `AddTool()`
- Event handler changes
- Many deprecated constants removed

#### STC (Styled Text Control) Changes
- Some API methods renamed/removed
- Style constants may differ

### 3. Standard Library Removals

#### Removed Modules
- `wxversion` (used in setup.py) - no Python 3 equivalent
- `commands` module (if used anywhere)

#### Changed Behavior
- `dict.iteritems()` → `dict.items()`
- `xrange()` → `range()`
- `basestring` → `str`

### 4. Build System Migration (MEDIUM PRIORITY)

#### py2exe → PyInstaller/cx_Freeze
- Complete rewrite of `setup.py` and `build.py`
- New manifest handling
- Different resource inclusion methods
- UPX compression still compatible

### 5. Binary/ctypes Issues (CRITICAL)

#### avisynth.py & avisynth_cffi.py
- Heavy use of ctypes for AviSynth DLL interaction
- Unicode path handling changed
- `isinstance(path, unicode)` checks need updates
- Struct packing may behave differently

### 6. Encoding Issues (CRITICAL)

#### File System Encoding
- `sys.getfilesystemencoding()` behavior changed
- Default encoding is UTF-8 in Python 3
- Need to audit all file I/O operations

---

## File-by-File Impact Analysis

### Core Files (23 Python files)

| File | Lines | Complexity | Priority | Key Issues |
|------|-------|------------|----------|-----------|
| **avsp.py** | ~18,906 | VERY HIGH | P0 | All import changes, print statements, exception handling, wxPython API |
| **wxp.py** | ~2,289 | HIGH | P0 | StringIO, cPickle, thread, wxPython changes |
| **avisynth.py** | ~1,254 | HIGH | P1 | Print statements, unicode handling, ctypes |
| **avisynth_cffi.py** | ? | HIGH | P1 | cffi updates, Python 3 compatibility |
| **pyavs.py** | ~880 | MEDIUM | P1 | Print statements, AviSynth interaction |
| **pyavs_avifile.py** | ? | MEDIUM | P2 | Print statements, Windows API |
| **setup.py** | ~270 | HIGH | P1 | Complete rewrite needed (py2exe → PyInstaller) |
| **build.py** | ? | MEDIUM | P1 | Build script updates |
| **i18n.py** | ~457 | LOW | P2 | String handling, file I/O |
| **global_vars.py** | ~43 | LOW | P3 | Minimal changes needed |
| **icons.py** | ? | LOW | P3 | Should be compatible |

### Macro Scripts
- **Location:** `macros/` directory
- **Count:** 12+ macro files
- **Impact:** Need Python 3 syntax updates
- **Priority:** P2 (after core functionality)

### Tool Scripts
- **Location:** `tools/` directory  
- **Files:** avs2avi_gui.py, encoder_gui.py, MP3Info.py, resize_calc.py, ToolsMenu.py
- **Priority:** P2

---

## Risk Assessment

### HIGH RISK
1. **wxPython Phoenix API changes** - May require significant refactoring of UI code
2. **AviSynth ctypes bindings** - Binary compatibility issues with Python 3
3. **Unicode/bytes handling** - Video frame data handling could break
4. **Build system** - Completely new packaging approach needed

### MEDIUM RISK
1. **Performance regressions** - Python 3 may be slower for some operations
2. **Third-party dependencies** - Some packages may be abandoned
3. **Windows registry access** - _winreg → winreg changes

### LOW RISK
1. **Syntax changes** - Mechanical, can be automated
2. **Standard library imports** - Well-documented migration path

---

## Testing Strategy

### Phase 1: Syntax Validation
- Use `2to3` tool for initial conversion
- Use `pyupgrade` for modern Python 3 idioms
- Run `flake8` / `pylint` for syntax errors

### Phase 2: Import Testing
- Test each module in isolation
- Verify all dependencies available for Python 3.14
- Create minimal test scripts for core functionality

### Phase 3: Integration Testing
- Test UI initialization
- Test video preview functionality
- Test AviSynth script loading/execution
- Test macro system

### Phase 4: Regression Testing
- Compare output with Python 2 version (if possible)
- Test all menu functions
- Test keyboard shortcuts
- Test all dialog boxes

---

## Compatibility Considerations

### Python 3.14 Specific Features
- **PEP 594** - Removed modules (already handled in 3.11+)
- **Performance improvements** - May benefit AvsPmod
- **Better error messages** - Easier debugging

### wxPython 4.2.x Requirements
- Supports Python 3.8+ (compatible with 3.14)
- pip-installable: `pip install wxPython`
- Active development continues

### Windows Compatibility
- Target Windows 10/11
- 64-bit primary, 32-bit secondary
- AviSynth+ compatibility (modern fork)

---

## Agent Specialization Recommendations

### Agent 1: Syntax Modernization Expert
**Focus:** Python 2→3 syntax transformations  
**Tools:** 2to3, pyupgrade, manual fixes  
**Scope:** All `.py` files  
**Deliverable:** Syntactically valid Python 3 code

### Agent 2: Dependency Migration Specialist  
**Focus:** Update all imports and third-party usage  
**Tools:** pip, dependency analysis  
**Scope:** Import statements, module usage patterns  
**Deliverable:** Working imports with Python 3 equivalents

### Agent 3: wxPython Phoenix Upgrader
**Focus:** UI framework migration  
**Tools:** wxPython 4.x documentation, migration guides  
**Scope:** All wx.* usage in avsp.py, wxp.py  
**Deliverable:** Functional UI with Phoenix API

### Agent 4: Build System Engineer
**Focus:** Packaging and distribution  
**Tools:** PyInstaller, setuptools  
**Scope:** setup.py, build.py, deployment scripts  
**Deliverable:** Working build process for Python 3

### Agent 5: AviSynth Integration Specialist
**Focus:** Video processing layer  
**Tools:** ctypes, cffi, AviSynth documentation  
**Scope:** avisynth.py, avisynth_cffi.py, pyavs*.py  
**Deliverable:** Working video preview and script execution

### Agent 6: Testing & QA Coordinator
**Focus:** Validation and regression testing  
**Tools:** pytest, manual testing  
**Scope:** All functionality  
**Deliverable:** Test suite and verified working application

---

## Success Criteria

### Minimum Viable Product (MVP)
✓ Application launches without errors  
✓ Can open/edit AviSynth script files  
✓ Syntax highlighting works  
✓ Can save files  

### Full Functionality
✓ Video preview displays correctly  
✓ All menu items functional  
✓ Macro system works  
✓ User sliders operational  
✓ Bookmarks functional  
✓ All tools work (crop editor, trim editor, etc.)  

### Production Ready
✓ No Python 3 deprecation warnings  
✓ Build creates standalone executable  
✓ Performance acceptable (within 20% of Python 2 version)  
✓ All original features working  
✓ Documentation updated  

---

## Estimated Effort

| Phase | Estimated Hours | Complexity |
|-------|----------------|------------|
| Syntax conversion | 20-30 | Medium |
| Import migration | 15-20 | Medium |
| wxPython upgrade | 40-60 | High |
| Build system | 20-30 | Medium |
| AviSynth bindings | 30-40 | High |
| Testing & debugging | 60-80 | High |
| **TOTAL** | **185-260 hours** | - |

**Timeline:** 4-6 weeks with one developer, or 2-3 weeks with team collaboration

---

## Blockers & Unknowns

### Known Blockers
- Need to verify AviSynth+ 64-bit DLL compatibility with Python 3
- Some wxPython AGW widgets may have changed significantly

### Unknown Factors
- Extent of custom ctypes structures that may break
- Performance impact of Python 3's Unicode handling on video frames
- Whether all macros use only supported Python features

### External Dependencies
- AviSynth+ must be installed (not in scope)
- FFmpeg/video codecs (not in scope)
- Windows system libraries (should be compatible)

---

## Recommendations

1. **Start with automated tools** (2to3, pyupgrade) for quick wins
2. **Test incrementally** - Don't convert everything at once
3. **Focus on core first** - Get main application working before macros/tools
4. **Create test harness** - Before breaking anything, establish baseline
5. **Document changes** - Keep migration notes for future reference
6. **Consider Python 3.10+** initially if 3.14 has issues (fallback plan)

---

## References

- [Python 2to3 Guide](https://docs.python.org/3/howto/pyporting.html)
- [wxPython Phoenix Migration](https://wxpython.org/Phoenix/docs/html/MigrationGuide.html)
- [What's New in Python 3.x](https://docs.python.org/3/whatsnew/)
- [PyInstaller Documentation](https://pyinstaller.org/)
- [AviSynth+ GitHub](https://github.com/AviSynth/AviSynthPlus)

---

**Document Version:** 1.0  
**Last Updated:** 2025-11-16  
**Author:** Migration Analysis Agent
