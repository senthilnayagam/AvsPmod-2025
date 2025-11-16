# Phase 4: Full Integration Testing Checklist

## Overview
Comprehensive testing of AvsPmod Python 3.14 migration with real-world usage scenarios.

## Test Environment
- **Python**: 3.14 / 3.12.9
- **OS**: Windows 11
- **wxPython**: 4.2.4
- **AviSynth**: AviSynth+ 3.7.3 (32-bit)

---

## 1. Application Launch ✅

### Basic Launch
- [ ] Application starts without crashes
- [ ] Main window appears
- [ ] No critical errors in console
- [ ] Menu bar loads correctly
- [ ] Toolbar loads correctly
- [ ] Status bar visible

**Test Command**:
```bash
python avsp.py
```

**Expected**: Application launches, shows empty editor

---

## 2. File Operations

### Create New Tab
- [ ] File → New Tab creates blank tab
- [ ] Tab shows "Untitled" or similar
- [ ] Can type in editor

### Open File
- [ ] File → Open dialog appears
- [ ] Can navigate directories
- [ ] Can select .avs file
- [ ] File content loads in editor
- [ ] Syntax highlighting appears

### Open Unicode Filename
- [ ] Can open `test_café_über_日本語.avs`
- [ ] Content displays correctly
- [ ] Unicode comments readable
- [ ] No encoding errors

### Save File
- [ ] File → Save works for existing file
- [ ] File → Save As shows dialog
- [ ] Can save with ASCII filename
- [ ] Can save with Unicode filename
- [ ] File content preserved

### Recent Files
- [ ] File → Recent Files shows history
- [ ] Can click to reopen file
- [ ] Unicode filenames display correctly

---

## 3. Editor Features

### Syntax Highlighting
- [ ] AviSynth keywords highlighted (BlankClip, Subtitle, etc.)
- [ ] Comments highlighted
- [ ] Strings highlighted
- [ ] Numbers highlighted

### Autocomplete
- [ ] Ctrl+Space shows autocomplete
- [ ] Function names appear
- [ ] Can select and insert

### Calltips
- [ ] Typing function shows calltip
- [ ] Function signature appears
- [ ] Parameter info visible

### Find/Replace
- [ ] Edit → Find opens dialog
- [ ] Can search text
- [ ] Find Next/Previous work
- [ ] Replace works
- [ ] Replace All works

### Undo/Redo
- [ ] Ctrl+Z undoes changes
- [ ] Ctrl+Y redoes changes
- [ ] Multiple undo levels work

---

## 4. AviSynth Integration

### Script Evaluation
- [ ] Can evaluate simple script (BlankClip)
- [ ] No AviSynth errors on valid script
- [ ] Error messages shown for invalid script

### Video Preview
- [ ] Video → Show video preview
- [ ] Preview window appears
- [ ] Video displays (if AviSynth working)
- [ ] Playback controls visible
- [ ] Frame slider works

### Frame Navigation
- [ ] Can go to specific frame number
- [ ] Next/Previous frame buttons work
- [ ] Frame counter updates
- [ ] Preview updates on navigation

### Video Info
- [ ] Video info shows resolution
- [ ] Shows frame count
- [ ] Shows FPS
- [ ] Shows color format

---

## 5. Bookmarks

### Add Bookmark
- [ ] Video → Set bookmark (or shortcut)
- [ ] Bookmark added to list
- [ ] Can add multiple bookmarks

### Navigate Bookmarks
- [ ] Can jump to bookmark
- [ ] Previous/Next bookmark work
- [ ] Bookmark list shows frame numbers

### Bookmark Titles
- [ ] Can add/edit bookmark title
- [ ] Unicode titles supported
- [ ] Titles saved with session

### Export Bookmarks
- [ ] Tools → Export bookmarks
- [ ] Can save to file
- [ ] Format correct (e.g., chapter format)

---

## 6. User Sliders

### Create Slider
- [ ] Can insert slider tag (e.g., `# Slider: value=50, min=0, max=100`)
- [ ] Slider appears in preview
- [ ] Can drag slider
- [ ] Value updates in script

### Slider Types
- [ ] Integer sliders work
- [ ] Float sliders work
- [ ] Color picker works (if implemented)
- [ ] Boolean toggles work

---

## 7. Macros

### List Macros
- [ ] Macros → Macros menu shows list
- [ ] Installed macros visible
- [ ] Folders organized correctly

### Run Macro
- [ ] Can execute simple macro
- [ ] Macro modifies script correctly
- [ ] No Python errors

### Test Specific Macros
- [ ] "Bookmarks at Intervals" works
- [ ] "DeleteFrame" works
- [ ] "DuplicateFrame" works
- [ ] Example macros run without errors

---

## 8. Tools

### Crop Editor
- [ ] Tools → Crop Editor opens
- [ ] Can select crop region (if video preview works)
- [ ] Crop values inserted into script

### Trim Editor
- [ ] Tools → Trim Editor opens
- [ ] Can select trim range
- [ ] Trim() command inserted

### Avisynth Function
- [ ] Can view function definitions
- [ ] Function help displays

---

## 9. Session Management

### Save Session
- [ ] File → Save Session
- [ ] Session file created (.ses)
- [ ] Unicode paths handled

### Load Session
- [ ] File → Load Session
- [ ] All tabs restored
- [ ] Bookmarks restored
- [ ] Window positions restored

### Auto-save
- [ ] Auto-save creates backup
- [ ] Can recover from crash (if feature exists)

---

## 10. Options/Preferences

### Open Options
- [ ] Options → Program settings
- [ ] Dialog appears
- [ ] All tabs load (General, Fonts, Colors, etc.)

### Modify Settings
- [ ] Can change font
- [ ] Can change colors
- [ ] Can modify paths
- [ ] Can change language

### Save Settings
- [ ] Click OK saves settings
- [ ] Settings persist after restart
- [ ] No encoding errors in config files

---

## 11. HTML Export

### Export HTML
- [ ] File → Export → HTML
- [ ] HTML file created
- [ ] Syntax highlighting in HTML
- [ ] Unicode content preserved
- [ ] Can open in browser

### Export Options
- [ ] Can customize export settings
- [ ] Different themes available

---

## 12. Translation/Internationalization

### Load Translation
- [ ] Options → Language selection
- [ ] Can switch languages
- [ ] UI updates to new language
- [ ] No missing translations (or shows key)

### Unicode in UI
- [ ] Menu items with Unicode display correctly
- [ ] Dialog boxes handle Unicode
- [ ] Tooltips work

---

## 13. Clipboard Operations

### Copy/Paste
- [ ] Ctrl+C copies text
- [ ] Ctrl+V pastes text
- [ ] Unicode text preserved
- [ ] Can paste from external apps

### Cut
- [ ] Ctrl+X cuts text
- [ ] Text removed from editor
- [ ] Available to paste

---

## 14. Window Management

### Multiple Tabs
- [ ] Can open multiple tabs
- [ ] Can switch between tabs
- [ ] Can close tabs
- [ ] Tab titles update correctly

### Split View
- [ ] Can split editor (if feature exists)
- [ ] Both views scroll independently

### Zoom
- [ ] Ctrl+Mouse wheel zooms text
- [ ] Zoom level persists per tab

---

## 15. Error Handling

### Invalid Script
- [ ] Shows AviSynth error message
- [ ] Error line highlighted (if supported)
- [ ] Can continue editing after error

### Missing Plugins
- [ ] Error message for missing plugin
- [ ] Doesn't crash application

### File Not Found
- [ ] Graceful handling of missing files
- [ ] Error dialog appears
- [ ] Can remove from recent files

---

## 16. Performance

### Large Files
- [ ] Can open large .avs files (>10 KB)
- [ ] Scrolling smooth
- [ ] Syntax highlighting not slow

### Long Scripts
- [ ] Scripts with 500+ lines work
- [ ] No lag in typing
- [ ] Search remains fast

---

## 17. Edge Cases

### Empty File
- [ ] Can save empty file
- [ ] Can reopen empty file
- [ ] No crashes

### Very Long Lines
- [ ] Lines with >1000 characters handled
- [ ] Horizontal scrolling works
- [ ] No display issues

### Special Characters in Path
- [ ] Files in paths with spaces work
- [ ] Paths with Unicode work
- [ ] Network paths work (if supported)

---

## Test Results Template

### Date: ___________
### Tester: ___________
### Python Version: ___________

| Category | Passed | Failed | Notes |
|----------|--------|--------|-------|
| Application Launch | ☐ | ☐ | |
| File Operations | ☐ | ☐ | |
| Editor Features | ☐ | ☐ | |
| AviSynth Integration | ☐ | ☐ | |
| Bookmarks | ☐ | ☐ | |
| User Sliders | ☐ | ☐ | |
| Macros | ☐ | ☐ | |
| Tools | ☐ | ☐ | |
| Session Management | ☐ | ☐ | |
| Options | ☐ | ☐ | |
| HTML Export | ☐ | ☐ | |
| Translation | ☐ | ☐ | |
| Clipboard | ☐ | ☐ | |
| Window Management | ☐ | ☐ | |
| Error Handling | ☐ | ☐ | |
| Performance | ☐ | ☐ | |
| Edge Cases | ☐ | ☐ | |

**Overall Status**: ☐ Pass ☐ Fail ☐ Partial

**Critical Blockers**:
- 

**Minor Issues**:
- 

**Notes**:
- 

---

## Automated Quick Tests

Run these for rapid smoke testing:

```bash
# 1. Launch test (5 seconds)
timeout /t 5 python avsp.py

# 2. Import test
python -c "import avsp; print('✅ avsp imports')"

# 3. wxPython test
python -c "import wx; print(f'✅ wxPython {wx.version()}')"

# 4. AviSynth test
python -c "import avisynth; env = avisynth.AVS_ScriptEnvironment(); print('✅ AviSynth works')"

# 5. Encoding test
python test_encoding.py
```

---

## Success Criteria

### Minimum (MVP)
- ✅ Application launches
- ✅ Can create/open/save files
- ✅ Editor works with basic features
- ✅ No critical crashes

### Full Functionality
- ✅ All file operations work
- ✅ AviSynth preview functional
- ✅ Bookmarks work
- ✅ Macros run successfully
- ✅ Settings save/load
- ✅ Unicode fully supported

### Production Ready
- ✅ All tests pass
- ✅ No known blockers
- ✅ Performance acceptable
- ✅ Documentation updated
- ✅ Build system works
