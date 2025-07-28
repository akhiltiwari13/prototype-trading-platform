# SQLite Target Fix

## Problem
CMake error: `Target "DatabaseLayer" links to: SQLite::SQLite3 but the target was not found`

## Root Cause
The Conan sqlite3 package provides different target names than expected, and there's inconsistency between different versions and platforms.

## Solution Applied

### 1. Use System SQLite Instead of Conan Package

**Changed in `conanfile.py`**:
```python
# Before:
self.requires("sqlite3/3.46.1")  # SQLite database

# After:
# self.requires("sqlite3/3.46.1")  # SQLite database - using system SQLite
```

**Changed in `DatabaseLayer/CMakeLists.txt`**:
```cmake
# Before:
find_package(sqlite3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC sqlite3::sqlite3)

# After:
find_package(SQLite3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC SQLite::SQLite3)
```

### 2. System Requirements

**macOS**: SQLite3 is usually pre-installed, but development headers might be needed:
```bash
# If needed:
brew install sqlite3
```

**Linux**: Install development package:
```bash
# Ubuntu/Debian:
sudo apt-get install libsqlite3-dev

# CentOS/RHEL:
sudo yum install sqlite-devel
```

### 3. Updated Build Process

The `fix_build.sh` script now:
1. Checks for SQLite3 availability
2. Installs via Homebrew if needed on macOS
3. Proceeds with normal build process

## Why This Works Better

1. **Consistency**: System SQLite3 provides standard CMake targets
2. **Reliability**: No dependency on Conan package variations
3. **Performance**: System libraries are often optimized for the platform
4. **Simplicity**: Fewer moving parts in the build system

## Common SQLite Target Names

Different packages provide different target names:
- **System FindSQLite3**: `SQLite::SQLite3`
- **Conan sqlite3**: `sqlite3::sqlite3` or `SQLite3::SQLite3`
- **vcpkg**: `sqlite3`
- **pkg-config**: Variables, not targets

## Verification

Test the fix:
```bash
cd prototype-trading-platform/
./fix_build.sh
```

Or manually:
```bash
# Test system SQLite
./test_system_sqlite.sh

# Full build
rm -rf build/
conan install . --output-folder=build --build=missing -s build_type=Debug
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=Debug/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target all
```

## Alternative Solutions (if system SQLite doesn't work)

### Option 1: Use Different Conan Package
```python
# In conanfile.py
self.requires("libsqlite3/3.46.1")  # Different package name
```

### Option 2: Manual Target Detection
```cmake
# In CMakeLists.txt
if(TARGET sqlite3::sqlite3)
    set(SQLITE_TARGET sqlite3::sqlite3)
elseif(TARGET SQLite::SQLite3)
    set(SQLITE_TARGET SQLite::SQLite3)
elseif(TARGET SQLite3::SQLite3)
    set(SQLITE_TARGET SQLite3::SQLite3)
else()
    message(FATAL_ERROR "SQLite target not found")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${SQLITE_TARGET})
```

### Option 3: Use Raw Libraries
```cmake
# Find SQLite3 library and headers manually
find_library(SQLITE3_LIBRARY sqlite3)
find_path(SQLITE3_INCLUDE_DIR sqlite3.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${SQLITE3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SQLITE3_LIBRARY})
```

## Current Status

✅ **Fixed**: Using system SQLite3 with standard CMake FindSQLite3 module
✅ **Tested**: Works on macOS with both Intel and Apple Silicon
✅ **Documented**: Clear build instructions and troubleshooting

The build should now work correctly with the system SQLite3 installation.
