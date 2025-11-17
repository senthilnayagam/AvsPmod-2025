# Video Playback Troubleshooting Guide

## Issue: Play Button Doesn't Work

The video playback controls (play/pause/navigation) require specific conditions to work properly.

### Prerequisites for Video Playback

1. **AviSynth Must Be Installed**
   - The application requires AviSynth 2.5+ to be installed on your system
   - Download from: http://avisynth.nl/

2. **Valid AviSynth Script Required**
   - You must have a valid `.avs` script open in the editor
   - The script must successfully load without errors

3. **Video Preview Must Be Visible**
   - The video preview window must be showing a valid frame
   - Check that the video preview is not showing an error message

### How to Test Video Playback

#### Method 1: Create a Simple Test Script

1. Click **File > New Tab**
2. Type or paste this simple AviSynth script:
   ```avisynth
   ColorBars(width=640, height=480)
   ```
3. Press **F5** to refresh the video preview
4. You should see a color bars test pattern in the video preview
5. Now try clicking the **Play** button (▶)

#### Method 2: Open an Existing Video File

1. Create a test script like:
   ```avisynth
   AVISource("C:\\path\\to\\your\\video.avi")
   # or
   DirectShowSource("C:\\path\\to\\your\\video.mp4")
   ```
2. Press **F5** to refresh
3. If the video appears in the preview, try the **Play** button

### Common Issues

#### Play Button Does Nothing
**Cause**: No valid video loaded or at last frame
- **Solution**: Make sure you have a valid AviSynth script open and the preview shows a frame
- **Solution**: If at the last frame, navigate to an earlier frame first

#### AviSynth Not Found Error
**Cause**: AviSynth is not installed or not detected
- **Solution**: Install AviSynth 2.5+ from http://avisynth.nl/
- **Solution**: Restart the application after installing

#### Preview Shows Error
**Cause**: Script syntax error or missing plugins
- **Solution**: Check the error message in the preview window
- **Solution**: Fix any syntax errors in your script
- **Solution**: Install required AviSynth plugins

### Keyboard Shortcuts for Playback

- **Ctrl+R**: Play/Pause video
- **Shift+Numpad +**: Double playback speed
- **Shift+Numpad -**: Halve playback speed
- **Shift+Numpad /**: Reset to normal speed
- **Shift+Numpad ***: Maximum speed
- **Shift+Numpad .**: Toggle drop frames mode

### Video Navigation (Always Available)

These controls work even without a valid video:
- **Left/Right Arrow**: Previous/Next frame
- **Home**: First frame
- **End**: Last frame
- **Page Up/Down**: Jump multiple frames
- **Slider**: Drag to scrub through video

### Technical Details

The `PlayPauseVideo()` function checks:
1. Is there a valid video preview showing?
2. Is the current clip an error clip?
3. Are we at the last frame?

If any of these conditions fail, the play button will appear to do nothing.

### Still Not Working?

If playback still doesn't work after:
1. ✅ AviSynth is installed
2. ✅ Valid script is open
3. ✅ Video preview shows a frame
4. ✅ Not at the last frame

Then there may be a deeper issue with:
- AviSynth DLL loading
- Python avisynth bindings (pyavs)
- Windows multimedia timer functions

Check the console output for error messages when starting the application.

### Alternative: External Player

If internal playback doesn't work, you can always use the **External Player** option:
- Press **F6**
- Or click the **External Player** button in the toolbar
- Configure the player in **File > Options > Program Settings > External player**

This will play your script in a separate video player (like VLC, MPC-HC, etc.).
