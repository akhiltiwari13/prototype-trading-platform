#!/bin/bash

# Comprehensive test runner for Alternate Trading Platform

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

# Run unit tests
print_header "Running Unit Tests"

cd build

# Run individual test suites
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
        if ./$test; then
            passed_tests+=("$test")
            print_success "$test passed"
        else
            failed_tests+=("$test")
            print_error "$test failed"
        fi
    else
        print_error "Test executable not found: $test"
        failed_tests+=("$test")
    fi
done

# Run CTest for comprehensive testing
print_info "Running CTest..."
if ctest --output-on-failure; then
    print_success "CTest passed"
else
    print_error "CTest failed"
fi

cd ..

# Summary
print_header "Test Summary"
echo "Passed tests: ${#passed_tests[@]}"
echo "Failed tests: ${#failed_tests[@]}"

if [ ${#failed_tests[@]} -eq 0 ]; then
    print_success "All tests passed!"
    exit 0
else
    print_error "Some tests failed:"
    for test in "${failed_tests[@]}"; do
        echo "  - $test"
    done
    exit 1
fi