# Troubleshooting Guide

## Common Compilation Issues

### "Too many errors emitted, stopping now"

This error typically indicates fundamental compilation problems. Here are the most common causes and solutions:

#### 1. Missing Dependencies

**Problem**: Conan dependencies not properly installed or configured.

**Solution**:
```bash
# Clean and reinstall dependencies
rm -rf build/
conan remove "*" --confirm
conan install . --output-folder=build --build=missing -s build_type=Release
```

#### 2. Header Include Issues

**Problem**: Headers not found or circular dependencies.

**Solution**:
- Check that all `#include` statements use the correct paths
- Ensure headers are properly installed in the include directories
- Verify CMakeLists.txt has correct `target_include_directories`

#### 3. C++ Standard Issues

**Problem**: Code uses C++20 features but compiler is set to older standard.

**Solution**:
```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(target_name PRIVATE cxx_std_20)
```

#### 4. Apple Silicon Specific Issues

**Problem**: Compiler flags not compatible with ARM64.

**Solution**:
```bash
# Use appropriate flags for Apple Silicon
export CXXFLAGS="-mcpu=apple-m1"
conan install . --build=missing -s arch=armv8 -s os=Macos
```

#### 5. Template/Namespace Issues

**Problem**: Template instantiation errors or namespace conflicts.

**Solution**:
- Check all namespace declarations are consistent
- Ensure template parameters are properly specified
- Verify forward declarations match implementations

## Specific Fixes Applied

### 1. OptionsGreeks Test Issues

**Fixed**:
- Reordered includes to put `gtest/gtest.h` first
- Added missing `<ctime>` include
- Updated CMakeLists.txt to include proper directories
- Changed GTest version to 1.14.0 for better compatibility

### 2. MarketDataProvider Issues

**Fixed**:
- Renamed `LadderDepth` constant to `LADDER_DEPTH` to avoid naming conflicts
- Added proper namespace qualifiers
- Fixed include dependencies

### 3. BlackScholesModel Issues

**Fixed**:
- Added `M_PI` definition for platforms that don't define it
- Ensured proper math constants are available

## Debugging Steps

### 1. Minimal Build Test

Run the test build script to isolate issues:
```bash
./test_build.sh
```

### 2. Verbose Compilation

Enable verbose output to see exact compilation commands:
```bash
cmake --build build --verbose
```

### 3. Individual Library Testing

Test each library separately:
```bash
# Test OptionsGreeks only
cmake --build build --target OptionsGreeks

# Test specific test
cmake --build build --target OptionsGreeksTests
```

### 4. Compiler-Specific Debugging

For Clang (default on macOS):
```bash
# Add debug flags
export CXXFLAGS="-v -g -O0"
cmake --build build
```

### 5. Dependency Verification

Check that all required packages are available:
```bash
conan list
conan info . --only=requires
```

## Platform-Specific Issues

### macOS Apple Silicon (M1/M2)

**Common Issues**:
- Rosetta 2 interference with native ARM64 builds
- Homebrew packages installed for wrong architecture
- Xcode command line tools not updated

**Solutions**:
```bash
# Ensure native ARM64 environment
arch -arm64 zsh
brew install --force cmake conan

# Verify architecture
uname -m  # Should show arm64
```

### macOS Intel

**Common Issues**:
- Mixed x86_64 and ARM64 dependencies
- Older Xcode versions

**Solutions**:
```bash
# Force x86_64 architecture
arch -x86_64 zsh
conan install . -s arch=x86_64
```

## Error Message Patterns

### Template Instantiation Errors

**Pattern**: `error: no matching function for call to...`

**Solution**: Check template parameter types and ensure all required headers are included.

### Linking Errors

**Pattern**: `undefined reference to...`

**Solution**: Verify all required libraries are linked in CMakeLists.txt.

### Header Not Found

**Pattern**: `fatal error: 'HeaderName.hpp' file not found`

**Solution**: Check include paths and ensure headers are installed correctly.

## Recovery Procedures

### Complete Clean Build

```bash
# Remove all build artifacts
rm -rf build/
rm -rf ~/.conan2/

# Reconfigure Conan
conan profile detect --force

# Fresh install
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build --preset conan-release
```

### Selective Rebuild

```bash
# Clean specific target
cmake --build build --target clean
cmake --build build --target OptionsGreeks
```

## Getting Help

If issues persist:

1. Check the exact error message in verbose mode
2. Verify all prerequisites are installed correctly
3. Test with a minimal example
4. Check platform-specific requirements
5. Consult the main README.md for additional setup instructions

## Useful Commands

```bash
# Check compiler version
clang++ --version

# Check CMake version
cmake --version

# Check Conan configuration
conan profile show default

# List installed packages
conan list

# Check system architecture
uname -m

# Verify Xcode tools
xcode-select -p
```