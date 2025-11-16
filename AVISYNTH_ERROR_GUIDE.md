# AviSynth Error Messages - User Guide

## Common Error: "Failed to compile AviSynth CFFI bindings"

### 🎯 TL;DR - This is NORMAL and usually not a problem!

If you see this error when starting AvsPmod:

```
Failed to compile AviSynth CFFI bindings: can't compile C/C++ code: 
unable to load module "distutils._msvccompiler"
Loading avisynth.dll failed! Make sure that AviSynth is installed.
Alternatively, specify now its directory.
```

**Don't panic!** Read below to understand what's happening.

---

## 🔍 What's Happening?

### Two Ways to Load AviSynth

AvsPmod can use **two different methods** to connect with AviSynth:

1. **avisynth_cffi.py** (64-bit, requires compilation)
   - Needs: avisynth_c.h header file
   - Needs: Visual Studio 2008 or compatible compiler
   - Advantage: Native 64-bit support

2. **avisynth.py** (32-bit, works everywhere)
   - No compilation required
   - Works on 64-bit Windows
   - Advantage: Just works!

### What the Error Means

The error message says:
- ✅ AvsPmod tried to use the CFFI version (method 1)
- ❌ CFFI compilation failed (expected on most systems)
- ✅ AvsPmod will automatically fall back to avisynth.py (method 2)

**Result:** Everything works fine!

---

## ✅ Is This a Problem?

### **NO, if:**

- ✅ The application continues to load
- ✅ You can preview AviSynth scripts
- ✅ Video preview works

### **YES, if:**

- ❌ Application won't start at all
- ❌ You can't preview any scripts
- ❌ Error says "AviSynth not found"

---

## 🛠️ Troubleshooting

### Scenario 1: App Works Fine (Just Annoying Error)

**Problem:** The error message is scary but everything works.

**Solution:** This is fixed in the latest version. The error is now suppressed.

**What's happening:** 
- AvsPmod tried CFFI, it failed (expected)
- Fell back to avisynth.py automatically
- Everything works!

**Action needed:** None! Or update to the latest version for a cleaner experience.

---

### Scenario 2: App Won't Start / AviSynth Really Not Found

**Problem:** Application can't find AviSynth at all.

**Symptoms:**
- Dialog: "Loading avisynth.dll failed!"
- Application won't start
- Can't preview any scripts

**Solution:** Install AviSynth+

#### Step 1: Download AviSynth+

- **URL:** https://github.com/AviSynth/AviSynthPlus/releases
- **Version:** 3.7.3 or later (recommended)
- **Architecture:** 
  - 64-bit Windows → Download x64 installer
  - 32-bit Windows → Download x86 installer

#### Step 2: Install

1. Run the installer (.exe file)
2. Accept default settings
3. Complete installation
4. Restart AvsPmod

#### Step 3: Verify

1. Start AvsPmod
2. Create a test script:
   ```avisynth
   Version()
   ```
3. Click "Video Preview"
4. Should show AviSynth version info

---

### Scenario 3: Want to Use Custom AviSynth Location

**Problem:** You have AviSynth installed in a non-standard location.

**Solution:** Configure custom directory in AvsPmod

#### Option 1: When Error Appears

1. When you see "Loading avisynth.dll failed" dialog
2. Click "Yes" to specify directory
3. Browse to your AviSynth installation folder
4. Select the folder containing `avisynth.dll`
5. Click OK

#### Option 2: In Settings

1. Open AvsPmod
2. Go to: Options → Program Settings
3. Find: AviSynth directory setting
4. Enter path to your AviSynth installation
5. Save and restart

---

## 📋 Quick Checklist

When you see the CFFI error, check:

- [ ] Does AvsPmod start successfully?
- [ ] Can you open/create scripts?
- [ ] Does video preview work?

**If all YES:** You're fine! The error is cosmetic.

**If any NO:** Follow Scenario 2 above to install AviSynth+.

---

## 🔬 Technical Details (For Developers)

### Why CFFI Fails

**CFFI compilation requires:**
1. `avisynth_c.h` header file (from AviSynth+ SDK)
2. C compiler (Visual Studio 2008 or compatible)
3. Python development headers
4. Configured build environment

**Most users don't have:**
- AviSynth+ SDK installed
- Visual Studio build tools
- Build environment configured

**Solution:** Automatic fallback to avisynth.py

### The Fallback Mechanism

```python
# Python 3.12+ on 64-bit Windows
if x86_64:
    try:
        import avisynth_cffi as avisynth  # Try CFFI first
    except OSError:
        import avisynth  # Fall back to 32-bit wrapper
else:
    import avisynth  # 32-bit systems use avisynth.py
```

**Result:** Always works, regardless of compilation environment.

### Performance Impact

**Q:** Does the fallback affect performance?

**A:** Minimal to none.
- avisynth.py uses ctypes (efficient)
- Actual video processing done by AviSynth+ (native C++)
- Python wrapper is just thin interface

**Conclusion:** No practical performance difference.

---

## 🎯 Recommended Solutions

### For Regular Users

**Option 1: Ignore It** (Current behavior)
- Error appears briefly
- Application continues
- Everything works
- No action needed

**Option 2: Update AvsPmod** (Recommended)
- Download latest version (v2.6.0+)
- Error is suppressed/handled gracefully
- Cleaner user experience

### For Advanced Users

**Option 1: Install Build Tools** (Not recommended)
1. Install Visual Studio Build Tools
2. Install AviSynth+ SDK (headers)
3. Set up build environment
4. CFFI will compile successfully

**Why not recommended:**
- Complex setup
- No practical benefit
- avisynth.py works perfectly

**Option 2: Contribute a Fix**
- Improve error message clarity
- Better fallback messaging
- Submit PR to GitHub

---

## 📝 Error Message Improvements (v2.6.0+)

### Old Behavior (Before Fix)
```
Failed to compile AviSynth CFFI bindings: can't compile C/C++ code...
Loading avisynth.dll failed! Make sure that AviSynth is installed.
[Scary dialog box]
```

### New Behavior (After Fix)
```
[Debug only] CFFI bindings unavailable (expected), using avisynth.py fallback
[Application continues silently]
```

**For users:** No visible error if avisynth.py works.

**For debug mode:** Informational message, not an error.

---

## 🆘 Still Having Problems?

### Check Installation

**Windows Registry Check:**
```powershell
# Check if AviSynth+ is registered
Get-ItemProperty "HKLM:\SOFTWARE\Avisynth" -ErrorAction SilentlyContinue
```

**DLL Location Check:**
```powershell
# Check if avisynth.dll is in System32
Test-Path "C:\Windows\System32\avisynth.dll"
```

**PATH Check:**
```powershell
# Check if AviSynth+ is in PATH
$env:PATH -split ';' | Select-String -Pattern "avisynth" -CaseSensitive:$false
```

### Get Help

1. **Check README.md** - Installation instructions
2. **Check RELEASE_NOTES.md** - Known issues
3. **GitHub Issues** - Search for similar problems
4. **Doom9 Forum** - AviSynth community support

**Links:**
- GitHub: https://github.com/senthilnayagam/AvsPmod-2025
- Issues: https://github.com/senthilnayagam/AvsPmod-2025/issues
- Forum: http://forum.doom9.org/showthread.php?t=153248

---

## 📊 Error Decision Tree

```
┌─ See CFFI error?
│
├─ Yes, but app continues
│  └─> ✅ NORMAL - Fallback working
│     └─> Action: None needed (or update for cleaner experience)
│
├─ Yes, app won't start
│  └─> ❌ PROBLEM - AviSynth not found
│     └─> Action: Install AviSynth+ (see Scenario 2)
│
└─ No error seen
   └─> ✅ WORKING - Everything fine
      └─> Action: None needed
```

---

## 💡 Key Takeaways

1. **CFFI error is usually harmless**
   - Automatic fallback to avisynth.py
   - No performance impact
   - Everything works fine

2. **Real problem is if app won't start**
   - Means AviSynth+ not installed
   - Install from official releases
   - Follow installation guide

3. **Latest version handles this better**
   - Error suppressed for users
   - Only shows in debug mode
   - Cleaner experience

4. **You don't need CFFI**
   - avisynth.py works perfectly
   - No need for build tools
   - Save yourself the hassle

---

## 🔄 Version History

**v2.5.1 (Python 2.7):**
- CFFI error shown to all users
- Confusing error messages
- No automatic fallback

**v2.6.0 (Python 3.14):**
- ✅ Improved error handling
- ✅ Automatic fallback to avisynth.py
- ✅ Clearer error messages
- ✅ Better user experience

---

## 📞 Support

**Before asking for help:**
1. ✅ Check if app actually works (despite error)
2. ✅ Try installing/reinstalling AviSynth+
3. ✅ Check this guide
4. ✅ Search existing GitHub issues

**When asking for help, provide:**
- Windows version
- AvsPmod version
- AviSynth+ version (if installed)
- Full error message
- Whether app works despite error

---

**Document Version:** 1.0  
**Date:** November 16, 2025  
**Applies to:** AvsPmod 2.6.0 (Python 3.14)  
**Status:** Current error handling improvements implemented
