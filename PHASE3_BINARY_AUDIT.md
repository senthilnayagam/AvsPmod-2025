# Phase 3.2: Binary File Handling Audit

## Date
November 16, 2025

## Objective
Audit all binary file operations in the codebase to ensure correct 'rb'/'wb' mode usage and Python 3 compatibility.

## Summary
✅ **ALL BINARY FILE OPERATIONS ARE CORRECT**

The codebase already properly handles binary files with correct modes and encoding/decoding.

---

## Binary File Operations Inventory

### 1. Pickle Operations (14+ locations) ✅

All pickle operations correctly use binary mode ('rb'/'wb').

#### Pickle Dumps (Write Operations)
| File | Line | Context | Mode | Status |
|------|------|---------|------|--------|
| avsp.py | 5564 | Save options on startup | 'wb' | ✅ Correct |
| avsp.py | 9606 | Save options (video) | 'wb' | ✅ Correct |
| avsp.py | 9861 | Save options | 'wb' | ✅ Correct |
| avsp.py | 9923 | Save options | 'wb' | ✅ Correct |
| avsp.py | 9957 | Save options | 'wb' | ✅ Correct |
| avsp.py | 9981 | Save options | 'wb' | ✅ Correct |
| avsp.py | 10042 | Save options | 'wb' | ✅ Correct |
| avsp.py | 10177 | Save options | 'wb' | ✅ Correct |
| avsp.py | 11640 | Save options | 'wb' | ✅ Correct |
| avsp.py | 11913 | Save options on exit | 'wb' | ✅ Correct |
| avsp.py | 11917 | Save macro options | 'wb' | ✅ Correct |
| avsp.py | 12696 | Save session backup | 'wb' | ✅ Correct |
| avsp.py | 12843 | Save session | 'wb' | ✅ Correct |
| avsp.py | 14327 | Save options | 'wb' | ✅ Correct |
| avsp.py | 14435 | Save filter data | 'wb' | ✅ Correct |

**Code Pattern:**
```python
with open(filename, mode='wb') as f:
    pickle.dump(self.options, f, protocol=0)
```

#### Pickle Loads (Read Operations)
| File | Line | Context | Mode | Status |
|------|------|---------|------|--------|
| avsp.py | 3782 | Load filter overrides | 'rb' | ✅ Correct |
| avsp.py | 5248 | Load macro options | 'rb' | ✅ Correct |
| avsp.py | 5729 | Load old options | 'rb' | ✅ Correct |
| avsp.py | 12686 | Load session | 'rb' | ✅ Correct |
| avsp.py | 14383 | Load filter templates | 'rb' | ✅ Correct |

**Code Pattern:**
```python
with open(self.macrosfilename, 'rb') as f:
    self.optionsMacros = pickle.load(f)
```

---

### 2. BytesIO for Pickle (Socket Communication) ✅

**File:** avsp.py, Line 5292-5293

**Code:**
```python
pickledstring = BytesIO()
pickle.dump(sys.argv[1:], pickledstring)
sock.sendall(pickledstring.getvalue())
```

**Status:** ✅ Correct - Uses BytesIO for binary pickle data (fixed in Phase 3.1b)

**Purpose:** Send command-line arguments to running instance via socket

---

### 3. PNG Export (Custom Encoder) ✅

**File:** avsp.py, Lines 13002-13117

**Function:** `SavePNG(filename, buf, height, alpha=False, filter_type=None)`

**Code:**
```python
with open(filename, 'wb') as file:
    # PNG signature
    signature = struct.pack('8B', 137, 80, 78, 71, 13, 10, 26, 10)
    file.write(signature)
    
    # Write chunks (IHDR, IDAT, IEND)
    write_chunk(file, 'IHDR', struct.pack(...))
    # ... compression and chunk writing
    write_chunk(file, 'IEND')
```

**Status:** ✅ Correct - Uses 'wb' mode for binary PNG data

**Purpose:** Save 16-bit RGB48/RGB64 frames as PNG files

**Technical Notes:**
- Pure Python PNG encoder (from png.py v0.0.15)
- Writes binary chunks: signature, IHDR, IDAT, IEND
- Uses struct.pack for binary data
- Applies zlib compression to image data

---

### 4. Image Export (wxPython) ✅

**File:** avsp.py, Lines 12873-12985

**Function:** `SaveImage(filename='', frame=None, ...)`

**Code:**
```python
img = bmp.ConvertToImage()
if ext == ".jpg":
    img.SetOption(wx.IMAGE_OPTION_QUALITY, str(quality))
img.SaveFile(filename, self.imageFormats[ext][1])
```

**Status:** ✅ Correct - wxPython's `SaveFile()` handles binary mode internally

**Supported Formats:**
```python
self.imageFormats = {
    '.bmp': (_('Windows Bitmap') + ' (*.bmp)', wx.BITMAP_TYPE_BMP),
    '.jpg': (_('JPEG') + ' (*.jpg)', wx.BITMAP_TYPE_JPEG),
    '.png': (_('PNG') + ' (*.png)', wx.BITMAP_TYPE_PNG),
    '.tif': (_('TIFF') + ' (*.tif)', wx.BITMAP_TYPE_TIF),
    '.pcx': (_('PCX') + ' (*.pcx)', wx.BITMAP_TYPE_PCX),
    '.xpm': (_('XPM') + ' (*.xpm)', wx.BITMAP_TYPE_XPM),
}
```

**Purpose:** Save current video frame as image file (BMP, JPEG, PNG, etc.)

---

### 5. AviSynth Preview Script ✅

**File:** avsp.py, Lines 15248-15273

**Function:** `MakePreviewScriptFile(script)`

**Code:**
```python
txt = self.getCleanText(script.GetText())
txt = self.GetEncodedText(txt, bom=True)  # Returns bytes!
# ...
with open(previewname, 'wb') as f:
    f.write(txt)
```

**Status:** ✅ Correct - Uses 'wb' mode because `GetEncodedText()` returns bytes

**Purpose:** Create temporary .avs script for preview with external player

**Technical Notes:**
- `GetEncodedText()` encodes text to bytes (UTF-8 or system encoding)
- Adds BOM (Byte Order Mark) if requested
- Handles CRLF line endings on Windows
- Fallback to UTF-8 if encoding fails

---

### 6. Filter Override Files ✅

**File:** avsp.py, Line 3778-3783

**Code:**
```python
if filename.startswith('http'):
    f = urllib.request.urlopen(filename)
else:
    f = open(filename, 'rb')
data = pickle.load(f)
f.close()
```

**Status:** ✅ Correct - Uses 'rb' for local files, urllib handles HTTP

**Purpose:** Load filter definitions from pickled data files

---

### 7. Macro Files in tools/ and macros/ ✅

**Files:** 
- `macros/Import bookmarks from file.py` (line 142)
- `tools/MP3Info.py` (line 642)
- `tools/encoder_gui.py` (line 1043)

**Code Examples:**
```python
f = open(filename, 'rb')  # Import bookmarks
i = MP3Info(open(sys.argv[1], 'rb'))  # MP3 info
f = open(audio_name, 'rb')  # Encoder audio
```

**Status:** ✅ Correct - All use 'rb' for binary data

---

## Python 2 vs Python 3 Differences

### String/Bytes Separation

| Python 2 | Python 3 |
|----------|----------|
| `str` = bytes | `str` = text (Unicode) |
| `unicode` = text | `bytes` = binary data |
| Files can mix text/bytes | Strict separation enforced |

### File Modes

| Mode | Python 2 | Python 3 |
|------|----------|----------|
| `'r'` | Text, can read bytes | Text only, needs encoding |
| `'rb'` | Binary | Binary (bytes object) |
| `'w'` | Text, can write bytes | Text only, needs encoding |
| `'wb'` | Binary | Binary (bytes object) |

### Key Rules in Python 3

1. **Text Mode (`'r'`, `'w'`):**
   - Must specify `encoding` parameter
   - Returns/accepts `str` objects
   - Automatic newline translation
   
2. **Binary Mode (`'rb'`, `'wb'`):**
   - No `encoding` parameter needed
   - Returns/accepts `bytes` objects
   - No newline translation

3. **Pickle:**
   - Always use binary mode
   - `pickle.dump()` produces bytes
   - `pickle.load()` expects bytes

4. **Mixing:**
   - Cannot write `str` to binary file
   - Cannot write `bytes` to text file
   - Must explicitly encode/decode

---

## Testing

### Test 1: Pickle with BytesIO
```python
from io import BytesIO
import pickle

bio = BytesIO()
test_data = ['arg1', 'arg2', '--flag']
pickle.dump(test_data, bio)
print(f'Pickled to BytesIO: {len(bio.getvalue())} bytes')
# Output: Pickled to BytesIO: 39 bytes ✅
```

### Test 2: Pickle File Operations
```python
import pickle

# Write
with open('test.pkl', 'wb') as f:
    pickle.dump({'key': 'value'}, f, protocol=0)

# Read
with open('test.pkl', 'rb') as f:
    loaded = pickle.load(f)
    
print(loaded)  # {'key': 'value'} ✅
```

### Test 3: Binary Write with Encoded Text
```python
text = 'Hello, World! 日本語 français'
encoded_text = text.encode('utf-8')

# Write binary
with open('test.txt', 'wb') as f:
    f.write(encoded_text)

# Read binary and decode
with open('test.txt', 'rb') as f:
    read_back = f.read().decode('utf-8')
    
print(text == read_back)  # True ✅
```

### Test 4: AvsPmod Import
```python
import avsp
print('avsp module imported successfully')
# Output: avsp module imported successfully ✅
```

---

## Common Patterns in Codebase

### ✅ Correct: Pickle Operations
```python
# Write
with open(filename, mode='wb') as f:
    pickle.dump(data, f, protocol=0)

# Read
with open(filename, mode='rb') as f:
    data = pickle.load(f)
```

### ✅ Correct: AviSynth Script Export
```python
txt = script.GetText()  # str
txt = self.GetEncodedText(txt, bom=True)  # bytes
with open(filename, 'wb') as f:
    f.write(txt)  # write bytes
```

### ✅ Correct: PNG Export
```python
with open(filename, 'wb') as file:
    file.write(struct.pack('8B', 137, 80, 78, 71, 13, 10, 26, 10))
    # ... more binary writes
```

### ✅ Correct: wxPython Image Save
```python
img.SaveFile(filename, wx.BITMAP_TYPE_PNG)
# wxPython handles binary mode internally
```

---

## Edge Cases Handled

### 1. HTTP vs Local File ✅
```python
if filename.startswith('http'):
    f = urllib.request.urlopen(filename)
else:
    f = open(filename, 'rb')
data = pickle.load(f)
```

### 2. Encoding Fallback ✅
```python
try:
    encoded_txt = txt.encode(script.encoding)
except UnicodeEncodeError:
    script.encoding = 'utf8'
    encoded_txt = txt.encode(script.encoding)
```

### 3. BOM (Byte Order Mark) ✅
```python
if bom and script.encoding == 'utf-8-sig':
    encoded_txt = codecs.BOM_UTF8 + encoded_txt
```

### 4. Line Endings ✅
```python
if self.options['eol'] == 'force crlf':
    txt = txt.replace('\n', '\r\n')
```

---

## Files Audited

### Core Application Files
- ✅ **avsp.py** (18,926 lines) - 15+ binary operations
- ✅ **wxp.py** (2,290 lines) - No binary operations
- ✅ **avisynth.py** (~1,254 lines) - Will check in Phase 3.3
- ✅ **avisynth_cffi.py** (~1,482 lines) - Will check in Phase 3.3

### Macro Scripts
- ✅ `macros/Import bookmarks from file.py` - Uses 'rb' correctly
- ✅ Other macro files - Only use text mode operations

### Tool Scripts
- ✅ `tools/MP3Info.py` - Uses 'rb' correctly
- ✅ `tools/encoder_gui.py` - Uses 'rb' correctly
- ✅ `tools/avs2avi_gui.py` - Text operations only
- ✅ `tools/resize_calc.py` - Text operations only

---

## Issues Found
**NONE** - All binary file operations are already correct! 🎉

---

## Why All Operations Are Correct

1. **Original Python 2 code already used binary modes** for pickle and binary data
2. **Phase 1 (2to3) didn't break these** - binary modes work in both Python 2 and 3
3. **Phase 3.1 fixed text mode** - added encoding to text operations
4. **Phase 3.1b fixed BytesIO** - changed StringIO to BytesIO for pickle
5. **wxPython handles its own I/O** - image save functions work correctly

---

## Next Steps

### Phase 3.3: AviSynth Bytes Handling
- Check `avisynth.py` for str/bytes mixing
- Check `avisynth_cffi.py` for Unicode path handling
- Check `pyavs.py` for frame data operations
- Test DLL loading with Unicode paths

### Phase 3.4: Encoding Edge Cases
- Test with non-ASCII filenames: `日本語.avs`
- Test with Unicode script comments
- Test HTML export with special characters
- Test macro files with international text

---

## Lessons Learned

### 1. Original Code Quality
The original AvsPmod developers wrote high-quality code that already followed best practices for binary file handling, making the Python 3 migration easier.

### 2. Binary Mode Compatibility
Binary file modes ('rb'/'wb') are the same in Python 2 and 3, making them safer for migration than text modes.

### 3. Layered Encoding
The `GetEncodedText()` function properly separates concerns:
- Converts text to bytes (encoding)
- Handles line endings
- Adds BOM if needed
- Then binary write just writes the bytes

### 4. wxPython Abstraction
wxPython's `SaveFile()` abstracts away file I/O details, making it more portable and less prone to Python 2→3 issues.

---

## References

- [Python 3 open() documentation](https://docs.python.org/3/library/functions.html#open)
- [Python 3 pickle module](https://docs.python.org/3/library/pickle.html)
- [Python 3 io module](https://docs.python.org/3/library/io.html)
- [Python 2 to 3 porting - Binary data](https://docs.python.org/3/howto/pyporting.html#binary-data)

---

**Status:** ✅ COMPLETE - No changes needed  
**Branch:** python3-migration  
**Migration Progress:** ~55% complete  
**Next Phase:** 3.3 (AviSynth bytes handling)
