#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
AvsPmod Build Script (Python 3.14)

Modern build system using PyInstaller instead of py2exe.
Creates standalone Windows executable with all dependencies.

Usage:
    python build_pyinstaller.py [--clean] [--upx] [--zip]

Options:
    --clean     Clean build directories before building
    --upx       Enable UPX compression (if UPX is installed)
    --zip       Create ZIP archive after build
    --help      Show this help message
"""

import os
import sys
import shutil
import subprocess
import zipfile
import platform
import argparse
from pathlib import Path

# Import version info
import global_vars
import i18n
import avsp


def print_banner():
    """Print build banner"""
    print("=" * 70)
    print(f"AvsPmod Build System - Python {sys.version_info.major}.{sys.version_info.minor}")
    print("=" * 70)
    print(f"Version: {global_vars.version}")
    print(f"Name: {global_vars.name}")
    print(f"Platform: {platform.system()} ({platform.machine()})")
    print(f"Python: {sys.version}")
    print("=" * 70)
    print()


def clean_build_dirs():
    """Remove build and dist directories"""
    print("🧹 Cleaning build directories...")
    
    dirs_to_clean = ['build', 'dist', '__pycache__']
    files_to_clean = ['*.pyc', '*.spec~']
    
    for dir_name in dirs_to_clean:
        if os.path.exists(dir_name):
            print(f"   Removing {dir_name}/")
            shutil.rmtree(dir_name)
    
    print("   ✅ Clean complete\n")


def check_pyinstaller():
    """Check if PyInstaller is installed"""
    try:
        import PyInstaller
        version = PyInstaller.__version__
        print(f"✅ PyInstaller {version} found")
        return True
    except ImportError:
        print("❌ PyInstaller not found!")
        print("   Install with: pip install pyinstaller")
        return False


def check_upx():
    """Check if UPX is available"""
    upx_paths = [
        os.path.join(os.environ.get('PROGRAMFILES', 'C:\\Program Files'), 'upx', 'upx.exe'),
        os.path.join(os.environ.get('PROGRAMFILES(X86)', 'C:\\Program Files (x86)'), 'upx', 'upx.exe'),
        shutil.which('upx'),
    ]
    
    for path in upx_paths:
        if path and os.path.isfile(path):
            print(f"✅ UPX found: {path}")
            return True
    
    print("⚠️  UPX not found (optional, used for compression)")
    return False


def update_translations():
    """Update translation files"""
    print("\n📝 Updating translations...")
    
    try:
        if i18n.main():
            print("   ✅ Translation master file updated")
            return True
        else:
            print("   ❌ Translation update failed")
            return False
    except Exception as e:
        print(f"   ⚠️  Translation update error: {e}")
        return False


def run_pyinstaller(clean=False, use_upx=False):
    """Run PyInstaller to create executable"""
    print("\n🔨 Building executable with PyInstaller...")
    
    # Build command
    cmd = [sys.executable, '-m', 'PyInstaller']
    
    if clean:
        cmd.append('--clean')
    
    if not use_upx:
        cmd.append('--noupx')
    
    cmd.append('avspmod.spec')
    
    print(f"   Command: {' '.join(cmd)}")
    print()
    
    try:
        result = subprocess.run(cmd, check=True)
        print("\n   ✅ Build successful!")
        return True
    except subprocess.CalledProcessError as e:
        print(f"\n   ❌ Build failed with exit code {e.returncode}")
        return False
    except Exception as e:
        print(f"\n   ❌ Build error: {e}")
        return False


def update_dist_files():
    """Update files in dist directory"""
    print("\n📦 Updating distribution files...")
    
    dist_dir = Path('dist') / 'AvsPmod'
    
    if not dist_dir.exists():
        print(f"   ❌ Distribution directory not found: {dist_dir}")
        return False
    
    try:
        # Update translation files with version info
        trans_dir = dist_dir / 'translations'
        if trans_dir.exists():
            i18n.UpdateTranslationFile(str(trans_dir), version=global_vars.version)
            print(f"   ✅ Updated translation files")
        
        # Create macros readme
        macros_dir = dist_dir / 'macros'
        if macros_dir.exists():
            readme_path = macros_dir / 'macros_readme.txt'
            avsp.GenerateMacroReadme(str(readme_path))
            print(f"   ✅ Created macros_readme.txt")
        
        return True
    except Exception as e:
        print(f"   ⚠️  Error updating files: {e}")
        return False


def create_zip_archive():
    """Create ZIP archive of the distribution"""
    print("\n📦 Creating ZIP archive...")
    
    x86_64 = sys.maxsize > 2**32
    arch = 'x86-64' if x86_64 else 'x86-32'
    
    zip_name = f'{global_vars.name}_v{global_vars.version}_({platform.system()}_{arch}).zip'
    
    dist_dir = Path('dist') / 'AvsPmod'
    
    if not dist_dir.exists():
        print(f"   ❌ Distribution directory not found: {dist_dir}")
        return False
    
    try:
        with zipfile.ZipFile(zip_name, 'w', zipfile.ZIP_DEFLATED) as zipf:
            for root, dirs, files in os.walk(dist_dir):
                for file in files:
                    file_path = Path(root) / file
                    arc_path = file_path.relative_to(dist_dir.parent)
                    zipf.write(file_path, arc_path)
                    print(f"   Adding: {arc_path}")
        
        file_size = os.path.getsize(zip_name) / (1024 * 1024)
        print(f"\n   ✅ Created: {zip_name} ({file_size:.2f} MB)")
        return True
    except Exception as e:
        print(f"   ❌ ZIP creation failed: {e}")
        return False


def print_summary(success, zip_created=False):
    """Print build summary"""
    print("\n" + "=" * 70)
    print("BUILD SUMMARY")
    print("=" * 70)
    
    if success:
        print("✅ Build completed successfully!")
        print(f"\n📂 Output directory: dist\\AvsPmod\\")
        print(f"🚀 Executable: dist\\AvsPmod\\AvsPmod.exe")
        
        if zip_created:
            x86_64 = sys.maxsize > 2**32
            arch = 'x86-64' if x86_64 else 'x86-32'
            zip_name = f'{global_vars.name}_v{global_vars.version}_({platform.system()}_{arch}).zip'
            print(f"📦 Archive: {zip_name}")
        
        print("\n💡 To test the application:")
        print("   cd dist\\AvsPmod")
        print("   .\\AvsPmod.exe")
    else:
        print("❌ Build failed!")
        print("\n💡 Troubleshooting:")
        print("   1. Check that all dependencies are installed")
        print("   2. Try with --clean option")
        print("   3. Check the error messages above")
    
    print("=" * 70)


def main():
    """Main build function"""
    parser = argparse.ArgumentParser(
        description='Build AvsPmod standalone executable',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('--clean', action='store_true',
                       help='Clean build directories before building')
    parser.add_argument('--upx', action='store_true',
                       help='Enable UPX compression')
    parser.add_argument('--zip', action='store_true',
                       help='Create ZIP archive after build')
    
    args = parser.parse_args()
    
    print_banner()
    
    # Check prerequisites
    if not check_pyinstaller():
        return 1
    
    if args.upx:
        check_upx()
    
    # Clean if requested
    if args.clean:
        clean_build_dirs()
    
    # Update translations
    update_translations()
    
    # Build executable
    if not run_pyinstaller(clean=args.clean, use_upx=args.upx):
        print_summary(False)
        return 1
    
    # Update distribution files
    update_dist_files()
    
    # Create ZIP if requested
    zip_created = False
    if args.zip:
        zip_created = create_zip_archive()
    
    # Print summary
    print_summary(True, zip_created)
    
    return 0


if __name__ == '__main__':
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        print("\n\n⚠️  Build interrupted by user")
        sys.exit(1)
    except Exception as e:
        print(f"\n\n❌ Unexpected error: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
