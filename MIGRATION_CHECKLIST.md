# AvsPmod Python 3.14 Migration Checklist

## Phase 0: Preparation ✓
- [x] Create agents.md analysis document
- [x] Create implementation plan
- [x] Create requirements.txt
- [ ] Create Python 3.14 virtual environment
- [ ] Install base dependencies
- [ ] Create migration branch in git
- [ ] Run initial 2to3 analysis

## Phase 1: Automated Syntax Conversion
### 1.1 Run 2to3 Tool
- [ ] Backup all files
- [ ] Run 2to3 on global_vars.py
- [ ] Run 2to3 on i18n.py
- [ ] Run 2to3 on icons.py
- [ ] Run 2to3 on avisynth.py
- [ ] Run 2to3 on avisynth_cffi.py
- [ ] Run 2to3 on pyavs.py
- [ ] Run 2to3 on pyavs_avifile.py
- [ ] Run 2to3 on wxp.py
- [ ] Run 2to3 on avsp.py (main file)
- [ ] Run 2to3 on build.py
- [ ] Run 2to3 on setup.py
- [ ] Commit each conversion

### 1.2 Apply Pyupgrade
- [ ] Install pyupgrade
- [ ] Run pyupgrade on all core files
- [ ] Run pyupgrade on macro files
- [ ] Run pyupgrade on tool files
- [ ] Commit modernizations

### 1.3 Fix Import Statements
- [ ] avsp.py: cPickle → pickle
- [ ] avsp.py: StringIO → io.StringIO
- [ ] avsp.py: urllib2 → urllib.request
- [ ] avsp.py: _winreg → winreg
- [ ] avsp.py: thread → _thread
- [ ] wxp.py: cPickle → pickle
- [ ] wxp.py: StringIO → io.StringIO
- [ ] wxp.py: thread → _thread
- [ ] avisynth.py: unicode checks
- [ ] All files: print statements
- [ ] Test imports work

## Phase 2: wxPython Phoenix Migration
### 2.1 Audit wxPython Usage
- [ ] Catalog all wx.* usage
- [ ] Document breaking changes
- [ ] Create compatibility notes

### 2.2 Update wxPython API Calls
- [ ] Remove wxversion.select() from setup.py
- [ ] Update wx.VERSION checks
- [ ] Fix SetToolTipString() → SetToolTip()
- [ ] Fix AddSimpleTool() → AddTool()
- [ ] Update event handlers
- [ ] Fix AGW widget usage
- [ ] Test application startup

### 2.3 Fix STC Issues
- [ ] Update STC constants
- [ ] Fix style methods
- [ ] Test text editor
- [ ] Test syntax highlighting

## Phase 3: Core Functionality Fixes
### 3.1 String/Bytes/Unicode
- [ ] Add encoding to all open() calls
- [ ] Fix isinstance(x, basestring) → str
- [ ] Fix isinstance(x, unicode) checks
- [ ] Update binary data handling
- [ ] Test with non-ASCII filenames

### 3.2 Integer Division
- [ ] Audit all / operators
- [ ] Update frame calculations to //
- [ ] Test frame seeking
- [ ] Test timeline positioning

### 3.3 Dictionary Iteration
- [ ] Verify .items() usage
- [ ] Verify .keys() usage
- [ ] Verify .values() usage
- [ ] Remove .iter* methods

## Phase 4: AviSynth Integration
### 4.1 avisynth.py (32-bit)
- [ ] Fix print statements
- [ ] Update unicode checks
- [ ] Test ctypes structures
- [ ] Test DLL loading
- [ ] Test clip creation

### 4.2 avisynth_cffi.py (64-bit)
- [ ] Update cffi version
- [ ] Fix Python 3 compatibility
- [ ] Test 64-bit DLL loading
- [ ] Test frame data access

### 4.3 pyavs.py and pyavs_avifile.py
- [ ] Fix print statements
- [ ] Update frame handling
- [ ] Test RGB/YUV conversion
- [ ] Test with video files

## Phase 5: Build System Migration
### 5.1 PyInstaller Setup
- [ ] Create PyInstaller spec file
- [ ] Add data files to spec
- [ ] Configure hidden imports
- [ ] Test basic build

### 5.2 Build Scripts
- [ ] Create new build.py
- [ ] Add UPX compression
- [ ] Create archive generation
- [ ] Test full build

### 5.3 Setup.py
- [ ] Remove py2exe code
- [ ] Add setuptools config
- [ ] Test pip install -e .

## Phase 6: Testing & Validation
### 6.1 Unit Tests
- [ ] Create tests/ directory
- [ ] Write import tests
- [ ] Write module tests
- [ ] Set up pytest
- [ ] All tests passing

### 6.2 Integration Tests
- [ ] Test application startup
- [ ] Test open .avs file
- [ ] Test syntax highlighting
- [ ] Test video preview
- [ ] Test all menus
- [ ] Test keyboard shortcuts
- [ ] Test user sliders
- [ ] Test bookmarks
- [ ] Test crop editor
- [ ] Test trim editor
- [ ] Test macro execution
- [ ] Test file save/load

### 6.3 Macro Migration
- [ ] Update Bookmarks at Intervals.py
- [ ] Update Bookmarks to Chapter.py
- [ ] Update DeleteFrame.py
- [ ] Update DuplicateFrame.py
- [ ] Update Import bookmarks.py
- [ ] Update Preview from current point.py
- [ ] Update Random Clip Order.py
- [ ] Update Save Image Sequence.py
- [ ] Update Shift Bookmarks.py
- [ ] Update all Example macros
- [ ] Test each macro

### 6.4 Tools Migration
- [ ] Update avs2avi_gui.py
- [ ] Update encoder_gui.py
- [ ] Update MP3Info.py
- [ ] Update resize_calc.py
- [ ] Update ToolsMenu.py
- [ ] Test all tools

## Phase 7: Performance & Polish
### 7.1 Performance
- [ ] Profile startup time
- [ ] Profile frame rendering
- [ ] Profile large scripts
- [ ] Optimize bottlenecks

### 7.2 Code Quality
- [ ] Run black formatter
- [ ] Run flake8 linter
- [ ] Fix linting issues
- [ ] Add type hints (optional)
- [ ] Update docstrings

### 7.3 Documentation
- [ ] Update README.md
- [ ] Update build_instructions_windows.txt
- [ ] Create CHANGELOG.md entry
- [ ] Update help files
- [ ] Create MIGRATION_NOTES.md

## Phase 8: Release Preparation
### 8.1 Version Bump
- [ ] Update global_vars.py version
- [ ] Update setup.py version
- [ ] Create changelog entry
- [ ] Create git tag

### 8.2 Build Releases
- [ ] Build Windows 32-bit
- [ ] Build Windows 64-bit
- [ ] Test both executables
- [ ] Create ZIP archives

### 8.3 GitHub Release
- [ ] Draft release notes
- [ ] Upload artifacts
- [ ] Document breaking changes
- [ ] Publish release

## Critical Issues to Track
- [ ] wxPython Phoenix compatibility
- [ ] AviSynth DLL loading
- [ ] Frame buffer handling
- [ ] Performance regressions
- [ ] Unicode filename support

## Known Blockers
- [ ] None currently identified

## Testing Priorities
1. Application launches
2. Can open/edit scripts
3. Video preview works
4. All menus functional
5. Macros work
6. No memory leaks
7. No crashes

---

**Last Updated:** 2025-11-16  
**Progress:** 0% (3/200+ tasks)  
**Status:** Ready to begin Phase 0.2
