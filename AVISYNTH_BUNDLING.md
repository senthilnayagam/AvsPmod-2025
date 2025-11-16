# Bundling AviSynth.dll with AvsPmod

## ❓ Question: Can we bundle AviSynth.dll with the executable?

**Short Answer:** Technically yes, but **NOT RECOMMENDED** for several important reasons.

---

## 🚫 Why NOT to Bundle AviSynth.dll

### 1. **Licensing Issues** ⚖️

**AviSynth+ License:** GPL v2 (same as AvsPmod)
- ✅ Compatible license
- ⚠️ BUT: Redistribution requires including full source code
- ⚠️ Must provide build instructions for AviSynth+
- ⚠️ Must clearly state which version is included

**Problem:** Users may not realize they're getting a bundled version, making updates difficult.

### 2. **Version Conflicts** 🔄

**AviSynth+ is frequently updated:**
- Users may have their own preferred version installed
- Plugins may require specific AviSynth+ versions
- System-wide AviSynth+ installation should take precedence
- Bundling could cause conflicts with existing installations

**Example Issue:**
```
User has AviSynth+ 3.7.3 installed system-wide
AvsPmod bundles AviSynth+ 3.7.2
→ Which version gets loaded?
→ Plugin compatibility issues!
```

### 3. **File Size** 📦

**AviSynth+ DLL size:**
- 32-bit: ~4-5 MB
- 64-bit: ~6-8 MB

**Not huge, but:**
- Adds unnecessary bloat if user already has it
- Increases download size
- Most users already have AviSynth+ installed

### 4. **Plugin Compatibility** 🔌

**AviSynth+ plugins expect:**
- System-registered AviSynth+ installation
- Specific plugin directories (auto-load)
- Registry entries for plugin paths

**Bundled AviSynth.dll:**
- Won't load system plugins automatically
- Users would need to configure plugin paths manually
- Breaks the "it just works" experience

### 5. **Maintenance Burden** 🛠️

**If you bundle AviSynth.dll:**
- Must update whenever AviSynth+ updates
- Must track security patches
- Must test compatibility with each update
- Must maintain build scripts for AviSynth+

### 6. **Legal/Compliance** 📜

**GPL v2 Redistribution Requirements:**
- Must provide complete source code
- Must provide build instructions
- Must clearly label versions
- Must maintain attribution
- Increases compliance burden

---

## ✅ Recommended Approach (Current Implementation)

### **Let Users Install AviSynth+ Separately**

**Advantages:**
1. ✅ Users choose their preferred version
2. ✅ Easy updates (independent of AvsPmod)
3. ✅ No version conflicts
4. ✅ Proper plugin support
5. ✅ Smaller download size
6. ✅ Less maintenance burden

**Current Implementation:**
```python
# avisynth.py line 53
if os.name == 'nt':
    path = os.path.join(directory, 'avisynth.dll')
    avidll = ctypes.WinDLL(path)  # Loads from system or custom path
```

**How it works:**
1. Checks `global_vars.avisynth_library_dir` for custom path
2. Falls back to system PATH
3. Loads from Windows system directories (C:\Windows\System32)

---

## 🔧 Alternative: Optional Bundling for Portable Version

If you really want to support portable usage, consider:

### **Option 1: Separate Portable Build**

Create two distributions:
- **Standard:** Requires AviSynth+ installed (recommended)
- **Portable:** Includes AviSynth.dll (clearly labeled, larger)

### **Option 2: Installer with AviSynth+ Option**

Use an installer (NSIS/Inno Setup) that:
- Checks if AviSynth+ is installed
- Offers to download/install AviSynth+ if missing
- Links to official AviSynth+ installer

### **Option 3: Custom AviSynth+ Directory Support (Current)**

Already implemented! Users can:
```python
# Set custom AviSynth+ directory in AvsPmod settings
global_vars.avisynth_library_dir = "C:\\Path\\To\\Custom\\AviSynth"
```

---

## 📝 How to Bundle (If You Really Want To)

### Step 1: Get AviSynth+ DLL

**Download from:**
- https://github.com/AviSynth/AviSynthPlus/releases
- Get the appropriate version (x86 or x64)
- Extract `avisynth.dll`

### Step 2: Modify avspmod.spec

```python
# Add to binaries section
binaries = [
    # Option 1: Bundle in root
    ('path/to/avisynth.dll', '.'),
    
    # Option 2: Bundle in subdirectory
    ('path/to/avisynth.dll', 'libs'),
]
```

### Step 3: Update avisynth.py to Check Bundle Location

```python
# Add to avisynth.py (around line 40)
def find_avisynth_dll():
    """Find AviSynth DLL in order of preference"""
    
    # 1. Custom directory (user override)
    if global_vars.avisynth_library_dir:
        custom_path = os.path.join(global_vars.avisynth_library_dir, 'avisynth.dll')
        if os.path.exists(custom_path):
            return custom_path
    
    # 2. Bundled with application (portable)
    if getattr(sys, 'frozen', False):
        # Running as PyInstaller bundle
        bundle_dir = sys._MEIPASS
        bundled_path = os.path.join(bundle_dir, 'avisynth.dll')
        if os.path.exists(bundled_path):
            return bundled_path
    
    # 3. System PATH (standard installation)
    return 'avisynth.dll'  # Let Windows find it

# Then use it:
path = find_avisynth_dll()
avidll = ctypes.WinDLL(path)
```

### Step 4: Update Documentation

**MUST include in README/documentation:**
```markdown
## Bundled AviSynth+ Notice

This distribution includes AviSynth+ version X.Y.Z

**License:** GPL v2
**Source Code:** https://github.com/AviSynth/AviSynthPlus
**Original Authors:** AviSynth+ development team

To use your own AviSynth+ installation instead:
- Set custom directory in AvsPmod settings
- OR remove the bundled avisynth.dll
```

### Step 5: Legal Compliance

**Required files to include:**
- `AVISYNTH_LICENSE.txt` - Full GPL v2 license
- `AVISYNTH_SOURCE.txt` - Link to source code
- `AVISYNTH_VERSION.txt` - Exact version bundled
- `AVISYNTH_AUTHORS.txt` - AviSynth+ credits

---

## 🎯 Recommendation for AvsPmod 2025

### **DO NOT BUNDLE** - Here's why:

1. **Target Audience:** AvsPmod users are AviSynth users
   - They already have AviSynth+ installed
   - They know how to install it
   - They prefer to manage their own AviSynth+ version

2. **Best Practice:** Separate concerns
   - AvsPmod = Script Editor
   - AviSynth+ = Video Processing Engine
   - Keep them independent

3. **User Experience:** Better without bundling
   - Users can update AviSynth+ independently
   - No version confusion
   - Plugins work out of the box

### **What to Do Instead:**

#### 1. Clear Installation Instructions

**Update README.md:**
```markdown
## Prerequisites

### AviSynth+ (Required)

AvsPmod requires AviSynth+ to preview videos.

**Download:** https://github.com/AviSynth/AviSynthPlus/releases

**Recommended:** AviSynth+ 3.7.3 or later

**Installation:**
1. Download the installer (x64 for 64-bit Windows)
2. Run the installer
3. Restart AvsPmod if already running

**Verify Installation:**
- AvsPmod will show an error if AviSynth+ is not found
- Check Windows PATH includes AviSynth+
```

#### 2. First-Run Detection

Add to AvsPmod:
```python
def check_avisynth_installed():
    """Check if AviSynth+ is available"""
    try:
        import avisynth
        return True
    except Exception as e:
        # Show friendly error message
        wx.MessageBox(
            "AviSynth+ not found!\n\n"
            "Please install AviSynth+ to use video preview.\n\n"
            "Download from:\n"
            "https://github.com/AviSynth/AviSynthPlus/releases",
            "AviSynth+ Required",
            wx.OK | wx.ICON_WARNING
        )
        return False
```

#### 3. Help Menu Link

Add menu item:
```
Help → Install AviSynth+ → Opens browser to download page
```

---

## 📊 Comparison

| Aspect | Bundled | Not Bundled (Recommended) |
|--------|---------|---------------------------|
| Download Size | Larger (+6-8 MB) | Smaller |
| Installation | One-step | Two-step (AviSynth+ then AvsPmod) |
| Updates | Coupled | Independent |
| Plugin Support | Complicated | Automatic |
| Version Control | User locked in | User chooses |
| Maintenance | High | Low |
| Legal Compliance | Complex | Simple |
| User Experience | Confusing | Clear |

---

## 🔮 Future Consideration

If you get requests for portable/bundled version:

1. **Survey users** - Do they actually want it?
2. **Create separate build** - Don't mix with standard release
3. **Clear labeling** - "AvsPmod Portable with AviSynth+"
4. **Document limitations** - Explain why standard is better

---

## 📋 Checklist: If You Decide to Bundle

- [ ] Download specific AviSynth+ version
- [ ] Update avspmod.spec with binary path
- [ ] Modify avisynth.py to check bundled location first
- [ ] Add AVISYNTH_LICENSE.txt (full GPL v2)
- [ ] Add AVISYNTH_SOURCE.txt (link to source)
- [ ] Add AVISYNTH_VERSION.txt (version info)
- [ ] Add AVISYNTH_AUTHORS.txt (credits)
- [ ] Update README with bundling notice
- [ ] Test plugin loading
- [ ] Test version conflicts
- [ ] Document how to use system AviSynth+ instead
- [ ] Create separate "portable" release variant
- [ ] Update RELEASE_NOTES.md with bundling info

---

## 💡 Conclusion

**For AvsPmod 2.6.0 (Python 3.14 release):**

**Recommendation: DO NOT BUNDLE**

**Reasoning:**
1. Target users already have AviSynth+ installed
2. Better user experience with separate installations
3. Less maintenance burden
4. Cleaner licensing
5. No version conflicts
6. Smaller distribution size
7. Standard practice in the AviSynth ecosystem

**Instead:**
- ✅ Document AviSynth+ requirement clearly
- ✅ Provide installation instructions
- ✅ Add first-run detection with helpful error
- ✅ Link to official AviSynth+ downloads

**This follows the Unix philosophy:** Do one thing and do it well. AvsPmod edits scripts, AviSynth+ processes video. Keep them separate!

---

## 📞 Questions?

If you have specific use cases requiring bundled AviSynth+:
1. Open a GitHub issue to discuss
2. Consider creating a separate "Portable" build
3. Survey users to see if there's demand

**Current Status:** Not bundled, and that's the right choice! ✅

---

**Document Version:** 1.0  
**Date:** November 16, 2025  
**Author:** Senthil Nayagam  
**License:** Same as AvsPmod (GPL v2)
