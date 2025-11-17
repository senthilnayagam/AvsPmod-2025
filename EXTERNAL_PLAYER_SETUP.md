# External Player Setup Guide

## Problem: VLC Shows "Unknown Codec" Error

VLC **cannot play AviSynth scripts** (`.avs` files) directly. AviSynth scripts are not video files - they're frameserver scripts that require special player support.

---

## ✅ Solution 1: Use MPC-HC (Recommended)

**MPC-HC (Media Player Classic - Home Cinema)** has built-in AviSynth support.

### Setup Steps:

1. **Download MPC-HC**
   - Download from: https://github.com/clsid2/mpc-hc/releases
   - Get the latest installer (e.g., `MPC-HC.1.9.x.x64.exe`)

2. **Install MPC-HC**
   - Run the installer
   - Default installation is fine
   - Note the installation path (usually `C:\Program Files\MPC-HC\mpc-hc64.exe`)

3. **Configure in AvsPmod**
   - In AvsPmod: **File > Options > Program Settings**
   - Find **"External player:"** field
   - Click **"..."** button and browse to:
     ```
     C:\Program Files\MPC-HC\mpc-hc64.exe
     ```
   - Click **OK**

4. **Test It**
   - Open any AviSynth script in AvsPmod
   - Press **F6** or click the **External Player** button
   - MPC-HC should open and play your script

---

## ✅ Solution 2: Use VirtualDub (Alternative)

**VirtualDub** is designed for AviSynth and works great.

### Setup Steps:

1. **Download VirtualDub**
   - Download from: https://sourceforge.net/projects/virtualdub/
   - Get `VirtualDub-1.10.4.zip` (or latest)

2. **Extract VirtualDub**
   - Extract to: `C:\Program Files\VirtualDub\`
   - Main executable: `VirtualDub64.exe` or `VirtualDub.exe`

3. **Configure in AvsPmod**
   - **File > Options > Program Settings**
   - **"External player:"** → Browse to:
     ```
     C:\Program Files\VirtualDub\VirtualDub64.exe
     ```
   - Click **OK**

4. **Test It**
   - Press **F6** to play script
   - VirtualDub opens with full editing capabilities

---

## ✅ Solution 3: Use AvsPmod's Internal Preview (Best)

**No external player needed!** AvsPmod has built-in video preview.

### Requirements:

1. **Install AviSynth**
   - Download: http://avisynth.nl/
   - Install AviSynth 2.5+ or AviSynth+ 3.x

2. **Use Internal Preview**
   - Create or open an `.avs` script
   - Press **F5** to refresh preview
   - Video appears in the preview pane
   - Use playback controls:
     - **Play/Pause**: Ctrl+R or click ▶ button
     - **Frame navigation**: Arrow keys
     - **Scrubbing**: Drag the frame slider

### Test Script:

```avisynth
# Paste this into a new tab
ColorBars(width=640, height=480, pixel_type="RGB24")
Subtitle("AvsPmod Internal Preview Test", size=32)
```

Press **F5** → You should see color bars with text

---

## Why VLC Doesn't Work

VLC is a general-purpose media player that:
- ❌ Cannot read AviSynth scripts
- ❌ Doesn't load AviSynth frameserver
- ❌ Only plays actual video files (MP4, AVI, MKV, etc.)

AviSynth scripts (`.avs`) are **script files**, not video files. They require:
- AviSynth frameserver to be installed
- Player with AviSynth input support (MPC-HC, VirtualDub)
- Or use AvsPmod's built-in preview

---

## Comparison: External Players

| Player | AviSynth Support | Best For | Download |
|--------|-----------------|----------|----------|
| **MPC-HC** | ✅ Built-in | Quick playback testing | [GitHub](https://github.com/clsid2/mpc-hc/releases) |
| **VirtualDub** | ✅ Native | Frame-accurate editing | [SourceForge](https://sourceforge.net/projects/virtualdub/) |
| **VLC** | ❌ No support | Regular video files only | N/A for AVS |
| **AvsPmod Internal** | ✅ Built-in | Script development | (Already included!) |

---

## Recommended Setup

**For script development:**
1. Use **AvsPmod's internal preview** (press F5)
2. Set **MPC-HC** as external player (press F6)
3. This gives you:
   - Fast preview in AvsPmod
   - Full-screen playback in MPC-HC
   - No need to save temporary files

---

## Quick Configuration Commands

### For MPC-HC (64-bit):
```
External player path: C:\Program Files\MPC-HC\mpc-hc64.exe
Extra args: (leave empty)
```

### For MPC-HC (32-bit):
```
External player path: C:\Program Files (x86)\MPC-HC\mpc-hc.exe
Extra args: (leave empty)
```

### For VirtualDub:
```
External player path: C:\Program Files\VirtualDub\VirtualDub64.exe
Extra args: /s (optional - silent mode)
```

---

## Troubleshooting

### "External player not found" error
- **Solution**: Click **"..."** in Options to browse for the player
- Verify the player is actually installed at that location

### Player opens but shows error
- **Solution**: Make sure AviSynth is installed
- Install from: http://avisynth.nl/
- Restart your system after installing AviSynth

### AvsPmod internal preview shows error
- **Solution**: Check your script syntax
- **Solution**: Install required AviSynth plugins
- **Solution**: Make sure AviSynth DLL is in System32

### Script works in AvsPmod but not external player
- **Solution**: Save your script first (Ctrl+S)
- **Solution**: Or enable **"Preview unsaved changes"** in Options

---

## Summary

✅ **Use MPC-HC** as external player (download and configure)  
✅ **Use AvsPmod internal preview** for development (requires AviSynth)  
❌ **Don't use VLC** - it can't play AviSynth scripts  

Press **F5** for internal preview, **F6** for external player.
