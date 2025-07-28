#!/bin/bash

# Debug test runner with detailed output

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_header() {
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_info() {
    echo -e "${YELLOW}[INFO]${NC} $1"
}

# Build the project first
print_header "Building Project"
if ! ./fix_build.sh; then
    print_error "Build failed"
    exit 1
fi
print_success "Build completed"

# Run individual test suites with detailed output
print_header "Running Individual Tests"

cd build

test_suites=(
    "tests/libraries/OptionsGreeks/OptionsGreeksTests"
    "tests/libraries/DatabaseLayer/DatabaseLayerTests"
    "tests/libraries/MarketDataProvider/MarketDataProviderTests"
    "tests/libraries/TradingEngine/TradingEngineTests"
)

failed_tests=()
passed_tests=()

for test in "${test_suites[@]}"; do
    if [ -f "$test" ]; then
        print_info "Running $test..."
        echo "----------------------------------------"
        
        # Run test with detailed output and capture result
        if ./$test --gtest_output=verbose 2>&1; then
            passed_tests+=("$test")
            print_success "$test passed"
        else
            failed_tests+=("$test")
            print_error "$test failed"
            echo "Test output above shows the failure details"
        fi
        echo "----------------------------------------"
    else
        print_error "Test executable not found: $test"
        failed_tests+=("$test")
    fi
done

cd ..

# Summary
print_header "Test Summary"
echo "Passed tests: ${#passed_tests[@]}"
echo "Failed tests: ${#failed_tests[@]}"

if [ ${#passed_tests[@]} -gt 0 ]; then
    print_success "Passed tests:"
    for test in "${passed_tests[@]}"; do
        echo "  ✅ $test"
    done
fi

if [ ${#failed_tests[@]} -gt 0 ]; then
    print_error "Failed tests:"
    for test in "${failed_tests[@]}"; do
        echo "  ❌ $test"
    done
    
    print_info "To debug individual tests, run:"
    for test in "${failed_tests[@]}"; do
        echo "  ./build/$test --gtest_output=verbose"
    done
    
    exit 1
else
    print_success "All tests passed!"
    exit 0
fi