# -*- mode: python ; coding: utf-8 -*-
"""
AvsPmod PyInstaller Spec File

Build standalone executable for Windows with PyInstaller (Python 3.14 compatible)
Replaces py2exe from Python 2.x

Usage:
    pyinstaller avspmod.spec

For clean build:
    pyinstaller --clean avspmod.spec
"""

import sys
import os
from PyInstaller.utils.hooks import collect_data_files, collect_submodules

# Determine architecture
x86_64 = sys.maxsize > 2**32
arch = 'x86-64' if x86_64 else 'x86-32'

# Import version info
import global_vars

block_cipher = None

# Data files to include
added_files = [
    ('filterdb.dat', '.'),
    ('README.md', '.'),
    ('changelog.txt', '.'),
    ('copying.txt', '.'),
    ('AvsP.ico', '.'),
    
    # Source files (for reference)
    ('run.py', 'src'),
    ('avsp.py', 'src'),
    ('wxp.py', 'src'),
    ('avisynth.py', 'src'),
    ('avisynth_cffi.py', 'src'),
    ('pyavs.py', 'src'),
    ('pyavs_avifile.py', 'src'),
    ('i18n.py', 'src'),
    ('icons.py', 'src'),
    ('global_vars.py', 'src'),
    
    # Help documentation
    ('help', 'help'),
    
    # Translations
    ('translations', 'translations'),
    
    # Macros
    ('macros', 'macros'),
    
    # Tools
    ('tools', 'tools'),
]

# Hidden imports that PyInstaller might miss
hiddenimports = [
    'wx',
    'wx.lib',
    'wx.lib.agw',
    'wx.lib.mixins',
    'wx.lib.mixins.listctrl',
    'wx.stc',
    'avisynth',
    'ctypes',
    'ctypes.wintypes',
    'glob',
    'shutil',
    'pickle',
    'io',
    'threading',
    'collections.abc',
]

# Binaries to include
binaries = []

# Analysis
a = Analysis(
    ['run.py'],  # Entry point
    pathex=[],
    binaries=binaries,
    datas=added_files,
    hiddenimports=hiddenimports,
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[
        # Exclude unused modules to reduce size
        'tkinter',
        'matplotlib',
        'numpy',
        'scipy',
        'pandas',
        'PIL',  # If not used
        'unittest',
        'test',
        'distutils',
    ],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

# Remove duplicate files
pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='AvsPmod',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,  # Compress with UPX if available
    console=False,  # GUI application, no console window
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon='AvsP.ico',
    version_file=None,  # TODO: Create version_info file
    uac_admin=False,
    uac_uiaccess=False,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='AvsPmod',
)
