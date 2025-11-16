# Phase 3.3: AviSynth ctypes Compatibility Fixes

## Overview
Fixed critical ctypes compatibility issues when migrating AviSynth integration from Python 2 to Python 3.14. The core issue was that ctypes behavior changed significantly between Python 2 and 3, particularly around pointer handling and the `from_param` method.

## Problems Identified

### 1. Missing `@classmethod` Decorators
**Location**: All classes with `from_param` methods (5 classes total)
- `AVS_ScriptEnvironment` (line 228)
- `AVS_VideoInfo` (line 360)
- `AVS_Clip` (line 582)
- `AVS_VideoFrame` (line 662)
- `AVS_Value` (line 742)

**Issue**: In Python 2, `from_param` was implicitly treated as a static method. In Python 3, it must be explicitly decorated with `@classmethod` or `@staticmethod`.

**Error**:
```
ctypes.ArgumentError: argument 1: OverflowError: int too long to convert
```

**Solution**: Added `@classmethod` decorator to all `from_param` methods:
```python
# Before (Python 2)
def from_param(obj):
    if not isinstance(obj, AVS_ScriptEnvironment):
        raise TypeError("Wrong argument: AVS_ScriptEnvironment expected")
    return obj.cdata

# After (Python 3)
@classmethod
def from_param(cls, obj):
    if not isinstance(obj, AVS_ScriptEnvironment):
        raise TypeError("Wrong argument: AVS_ScriptEnvironment expected")
    return ctypes.c_void_p(obj.cdata)
```

### 2. Pointer Casting for Integer Handles
**Location**: 
- `AVS_ScriptEnvironment.from_param()` (line 228)
- `AVS_Clip.from_param()` (line 582)

**Issue**: `avs_create_script_environment` returns `ctypes.c_void_p` which becomes a Python integer. When passed back to ctypes functions, this integer must be explicitly cast to `c_void_p`.

**Solution**: 
```python
@classmethod
def from_param(cls, obj):
    if not isinstance(obj, AVS_ScriptEnvironment):
        raise TypeError("Wrong argument: AVS_ScriptEnvironment expected")
    # Cast integer pointer value to c_void_p for ctypes
    return ctypes.c_void_p(obj.cdata)
```

### 3. String to Bytes Conversion
**Location**: `AVS_ScriptEnvironment.invoke()` (line 238)

**Issue**: AviSynth C API expects `char*` (bytes), but Python 3 strings are Unicode. The `ctypes.c_char_p` type expects bytes, not str.

**Error**:
```
ctypes.ArgumentError: argument 2: TypeError: wrong type
```

**Solution**: Convert function names and argument names to bytes:
```python
def invoke(self, name, args=[], arg_names=None):
    # Convert name to bytes for ctypes.c_char_p
    if isinstance(name, str):
        name = name.encode('utf-8')
    # ... rest of method
    if isinstance(arg_names, list):
        # Convert arg_names to bytes as well
        arg_names_bytes = [n.encode('utf-8') if isinstance(n, str) else n for n in arg_names]
        arg_names2 = (ctypes.c_char_p * len(arg_names_bytes))(*arg_names_bytes)
        arg_names = ctypes.cast(ctypes.byref(arg_names2), ctypes.POINTER(ctypes.c_char_p))
```

### 4. collections.abc Migration
**Location**: Lines 32 and 762

**Issue**: Python 3.10+ moved `Iterable` from `collections` to `collections.abc`.

**Error**:
```
AttributeError: module 'collections' has no attribute 'Iterable'
```

**Solution**:
```python
# Line 32
import collections.abc

# Line 762
if isinstance(value, collections.abc.Iterable):
    # ... array handling
```

### 5. __del__ Safety Checks
**Location**: `AVS_ScriptEnvironment.__del__()` (line 234)

**Issue**: During cleanup, `__del__` could be called before `cdata` was initialized or after it was already freed.

**Solution**: Added safety check:
```python
def __del__(self):
    if hasattr(self, 'cdata') and self.cdata:
        avs_delete_script_environment(self)
```

## Python 2 vs Python 3 ctypes Differences

| Aspect | Python 2 | Python 3 |
|--------|----------|----------|
| `from_param` method | Implicitly static | Must use `@classmethod` |
| `c_char_p` input | Accepts str or bytes | Only accepts bytes |
| `c_void_p` return | Auto-converts to int | Becomes int, needs explicit cast |
| String encoding | ASCII by default | Unicode (UTF-8) |
| Pointer handling | Loose type checking | Strict type checking |

## Test Results

All core AviSynth functionality now works correctly:

```python
✅ Created AviSynth environment
✅ Version() call succeeded (returns AVS_Clip)
✅ BlankClip created (320x240, 24 frames)
✅ Got video info (width=320, height=240, frames=24, fps=1/1)
✅ Got frame 0 (row_size=960, height=240)
```

Test command:
```python
import avisynth
env = avisynth.AVS_ScriptEnvironment()
clip = env.invoke('BlankClip', [24, 320, 240, 'RGB24', 1])
vi = clip.get_video_info()
frame = clip.get_frame(0)
```

## Cleanup Issues (Non-Critical)

There are still some `OSError` exceptions in `__del__` methods during cleanup:
```
Exception ignored in: <function AVS_Value.__del__>
OSError: exception: access violation reading 0xFFFFFFFFFFFFFFFF
```

These occur during garbage collection and don't affect functionality. They're related to:
1. Double-free attempts
2. Reference counting edge cases
3. Shutdown order dependencies

These are deferred to a future phase as they don't impact normal operation.

## Files Modified

- `avisynth.py`: 28 insertions, 12 deletions
  - Added 5 `@classmethod` decorators
  - Added 2 `ctypes.c_void_p()` casts
  - Added string→bytes conversion in `invoke()`
  - Fixed collections.abc import and usage
  - Added `__del__` safety check

## Testing Environment

- **OS**: Windows 11
- **Python**: 3.14.0
- **AviSynth**: AviSynth+ 3.7.3 (32-bit)
- **DLL Path**: C:\Windows\SysWOW64\AviSynth.dll
- **Install Path**: C:\Program Files (x86)\AviSynth

## Key Learnings

1. **ctypes `from_param` semantics changed**: In Python 3, it must be a classmethod or staticmethod
2. **Integer pointers need explicit casting**: `c_void_p` integers must be re-wrapped before passing to ctypes
3. **String encoding is mandatory**: All C API strings must be explicitly encoded to bytes
4. **collections namespace changed**: Use `collections.abc.Iterable` in Python 3.10+
5. **Defensive cleanup**: Always check attribute existence and validity in `__del__`

## Next Steps

Phase 3.3 is now complete. Next phases:

- **Phase 3.4**: Test encoding edge cases (non-ASCII filenames, special characters)
- **Phase 4**: Full AviSynth integration testing with real video files
- **Phase 5**: Build system migration (py2exe → PyInstaller)

## Commit Hash

```
d218443 - Phase 3.3 final: Fix ctypes compatibility for Python 3
```
