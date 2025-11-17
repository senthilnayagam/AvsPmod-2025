# GUI Launch Troubleshooting Status

**Date:** November 17, 2025  
**Status:** In Progress - Pausing for the night

## Problem
AvsPmod.exe launches without errors but GUI window is not visible on screen.

## Fixes Applied Today (6 commits)

### 1. ✅ CFFI Error Suppression (Commit: 17a8c6d)
- Suppressed scary CFFI compilation error message
- Added automatic fallback to avisynth.py
- Created AVISYNTH_ERROR_GUIDE.md

### 2. ✅ PyInstaller pkg_resources (Commit: 32caf32)
- Excluded setuptools/pkg_resources from build
- Fixed FileNotFoundError during build

### 3. ✅ function_exists() Encoding (Commit: 0f9423d)
- Fixed ctypes.ArgumentError in avisynth.py line 279
- Added string-to-bytes conversion for function names

### 4. ✅ Plugin Names Decoding (Commit: d57fb0e)
- Fixed TypeError: can't concatenate str with bytes
- Decode plugin function names from get_var()

### 5. ✅ Bitmap Mask Creation (Commit: 51c0147)
- Fixed wxAssertionError for bitmap mask
- Use wx.Mask(bitmap, wx.WHITE) for wxPython 4.x

### 6. ✅ Final Distribution ZIP Created
- AvsPmod_v2.5.1_(Windows_x86-64).zip (19.27 MB)
- Executable: dist\AvsPmod\AvsPmod.exe

## Current Status

### What's Working ✅
- Executable builds successfully
- No Python errors during launch
- Process starts and exits cleanly
- All encoding issues fixed
- All wxPython bitmap issues fixed

### What's NOT Working ❌
- **GUI window does not appear on screen**
- Process launches but no visible window
- No error messages in logs

## Last Test Output

```
Error parsing TimeStretch_TimeStretch plugin parameters: unknown character...
(Multiple plugin parsing warnings - non-critical)

avsp.py:5313: wxPyDeprecationWarning: Call to deprecated item. Use SetSize instead.
avsp.py:5334: wxPyDeprecationWarning: Using deprecated class. Use DropTarget instead.
avsp.py:7081: wxPyDeprecationWarning: Call to deprecated item EmptyBitmap...
avsp.py:7088: wxPyDeprecationWarning: Call to deprecated item EmptyBitmap...

Using AviSynth from PATH
CFFI bindings unavailable (expected), using 32-bit avisynth.py fallback
```

**No crash errors! But also no GUI visible.**

## Possible Causes to Investigate Tomorrow

### 1. Window Creation Issue
- Window may be created but off-screen or minimized
- Window may be hidden behind other windows
- Window size/position might be invalid

### 2. wx.App Main Loop
- Main loop might not be running
- App might be exiting immediately after window creation
- Event loop issue

### 3. Display/Monitor Issue
- Window might be on different monitor
- Window might be at position outside screen bounds
- DPI scaling issue on Windows 11

### 4. Executable vs Development Mode
- Might work when run from Python but not as .exe
- PyInstaller windowing mode issue (using runw.exe bootloader)

## Next Steps for Tomorrow

1. **Test with development Python first:**
   ```powershell
   cd c:\Users\senth\code\AvsPmod-2025
   python run.py
   ```
   If this works, the issue is PyInstaller-specific.

2. **Check if window is created but hidden:**
   ```powershell
   Get-Process | Where-Object { $_.ProcessName -eq "AvsPmod" } | 
       Select-Object ProcessName, MainWindowTitle, MainWindowHandle, Responding
   ```

3. **Try console mode executable:**
   - Modify avspmod.spec to use console mode instead of windowed
   - See if any console output appears

4. **Add debug prints in avsp.py:**
   - Add print statements in MainFrame.__init__
   - Add print after self.Show()
   - See how far initialization gets

5. **Check wxPython app creation:**
   - Verify wx.App is created properly
   - Check if MainLoop() is called
   - Look for early exit conditions

6. **Test minimal wxPython window:**
   - Create simple test script with just wx window
   - Build with PyInstaller
   - See if basic GUI works

## Files Modified Today

1. `avisynth.py` - function_exists() encoding fix
2. `avsp.py` - Plugin decoding + bitmap mask fixes  
3. `pyavs.py` - CFFI fallback
4. `avspmod.spec` - pkg_resources exclusion
5. `AVISYNTH_ERROR_GUIDE.md` - User documentation
6. `AVISYNTH_BUNDLING.md` - Technical documentation

## Build Info

- **Python:** 3.12.9 (Conda)
- **PyInstaller:** 6.16.0
- **wxPython:** 4.2.x (Phoenix)
- **Platform:** Windows 11 AMD64
- **Output:** dist\AvsPmod\AvsPmod.exe
- **Size:** ~19 MB (with all dependencies)

## Useful Commands for Tomorrow

```powershell
# Run from source (test if GUI works without PyInstaller)
cd c:\Users\senth\code\AvsPmod-2025
python run.py

# Check running processes
Get-Process AvsPmod -ErrorAction SilentlyContinue

# Clean rebuild
python build_pyinstaller.py --clean

# Test executable with error capture
cd dist\AvsPmod
.\AvsPmod.exe 2>&1 | Tee-Object debug.log
```

## Notes

- All Python 2→3 encoding issues are fixed
- Application initializes without crashes
- This appears to be a wxPython GUI display issue, not a Python code issue
- The fixes applied today are all valid and necessary
- The remaining issue is likely windowing/display related

---
**Resume tomorrow with fresh perspective!** 🌙
