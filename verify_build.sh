#!/bin/bash

# Comprehensive build verification

set -e

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Build the project
print_info "Building complete project..."
if ./fix_build.sh; then
    print_success "Build completed successfully"
else
    print_error "Build failed"
    exit 1
fi

# Verify all executables exist
print_info "Verifying executables..."

executables=(
    "build/applications/TradingEngineApp/TradingEngineApp"
    "build/applications/MarketDataApp/MarketDataApp"
    "build/tests/libraries/OptionsGreeks/OptionsGreeksTests"
    "build/tests/libraries/DatabaseLayer/DatabaseLayerTests"
    "build/tests/libraries/MarketDataProvider/MarketDataProviderTests"
    "build/tests/libraries/TradingEngine/TradingEngineTests"
)

missing_executables=()

for exe in "${executables[@]}"; do
    if [ -f "$exe" ]; then
        print_success "Found: $exe"
    else
        print_error "Missing: $exe"
        missing_executables+=("$exe")
    fi
done

# Check GUI executable (optional)
if [ -f "build/applications/TradingFrontendApp/TradingFrontendApp" ]; then
    print_success "Found: GUI application"
else
    print_info "GUI application not built (SDL2 may not be available)"
fi

# Summary
if [ ${#missing_executables[@]} -eq 0 ]; then
    print_success "All required executables built successfully!"
    print_info "Platform is ready to use"
    echo ""
    echo "Next steps:"
    echo "  ./run_tests.sh      # Run all unit tests"
    echo "  ./run_simulation.sh # Start trading simulation"
else
    print_error "Some executables are missing:"
    for exe in "${missing_executables[@]}"; do
        echo "  - $exe"
    done
    exit 1
fi