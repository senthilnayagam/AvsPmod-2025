"""
Test script to diagnose file dialog and video playback issues
Run this with: python test_gui_issues.py
"""

import sys
import os

# Add parent directory to path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

print("=" * 70)
print("AvsPmod GUI Issue Diagnostic Test")
print("=" * 70)

# Test 1: Import wxPython
print("\n1. Testing wxPython import...")
try:
    import wx
    print(f"   ✅ wxPython {wx.VERSION_STRING} imported successfully")
    print(f"   Platform: {wx.PlatformInfo}")
except ImportError as e:
    print(f"   ❌ Failed to import wxPython: {e}")
    sys.exit(1)

# Test 2: Check FileDialog
print("\n2. Testing wx.FileDialog availability...")
try:
    assert hasattr(wx, 'FileDialog')
    print("   ✅ wx.FileDialog is available")
    
    # Test creating a dialog (without showing it)
    app = wx.App(False)
    dlg = wx.FileDialog(None, "Test", "", "", "*.*", wx.FD_OPEN)
    print("   ✅ wx.FileDialog can be instantiated")
    dlg.Destroy()
    app.Destroy()
except Exception as e:
    print(f"   ❌ wx.FileDialog test failed: {e}")

# Test 3: Check constants
print("\n3. Testing wx constants...")
constants_to_check = [
    'ID_OK', 'ID_CANCEL', 'OPEN', 'SAVE', 'FILE_MUST_EXIST',
    'OVERWRITE_PROMPT', 'MULTIPLE', 'FD_OPEN', 'FD_SAVE'
]
for const in constants_to_check:
    if hasattr(wx, const):
        value = getattr(wx, const)
        print(f"   ✅ wx.{const} = {value}")
    else:
        print(f"   ⚠️  wx.{const} not found")

# Test 4: Check button/bitmap classes
print("\n4. Testing wx.Button and wx.Bitmap...")
try:
    assert hasattr(wx, 'Button')
    assert hasattr(wx, 'Bitmap')
    assert hasattr(wx, 'BitmapButton')
    print("   ✅ wx.Button, wx.Bitmap, wx.BitmapButton available")
except Exception as e:
    print(f"   ❌ Button/Bitmap test failed: {e}")

# Test 5: Check EVT_BUTTON
print("\n5. Testing wx.EVT_BUTTON...")
try:
    assert hasattr(wx, 'EVT_BUTTON')
    print(f"   ✅ wx.EVT_BUTTON available: {wx.EVT_BUTTON}")
except Exception as e:
    print(f"   ❌ EVT_BUTTON test failed: {e}")

# Test 6: Test actual FileDialog with ShowModal
print("\n6. Testing FileDialog.ShowModal() return values...")
try:
    app = wx.App(False)
    # Create dialog but don't show it - just check the method exists
    dlg = wx.FileDialog(None, "Test", "", "", "*.*", wx.FD_OPEN)
    assert hasattr(dlg, 'ShowModal')
    assert hasattr(dlg, 'GetPath')
    assert hasattr(dlg, 'GetPaths')
    print("   ✅ ShowModal(), GetPath(), GetPaths() methods exist")
    dlg.Destroy()
    app.Destroy()
except Exception as e:
    print(f"   ❌ FileDialog methods test failed: {e}")

# Test 7: Check for known wxPython 4.x API changes
print("\n7. Checking wxPython 4.x API compatibility...")
compatibility_checks = [
    ('wx.FD_OPEN', 'Modern file dialog style'),
    ('wx.OPEN', 'Legacy file dialog style (deprecated)'),
]
for attr, desc in compatibility_checks:
    parts = attr.split('.')
    obj = wx
    try:
        for part in parts[1:]:
            obj = getattr(obj, part)
        print(f"   ✅ {attr} available ({desc})")
    except AttributeError:
        print(f"   ❌ {attr} NOT available ({desc})")

print("\n" + "=" * 70)
print("Diagnostic complete!")
print("=" * 70)

print("\n📋 RECOMMENDATIONS:")
print("1. Check if OnMenuFileOpen() is actually being called")
print("2. Add debug print statements to OpenFile() method")
print("3. Check for exception handlers that might be swallowing errors")
print("4. Verify event bindings in __init__ method")
print("5. Test with a minimal wxPython file dialog example")
