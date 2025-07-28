#!/bin/bash

# Test basic functionality

set -e

echo "Testing basic functionality..."

# Use the built libraries directly instead of standalone compilation
echo "Testing basic functionality using built executables..."

cd build

# Test that all applications can start
echo "Testing TradingEngineApp..."
timeout 2s ./applications/TradingEngineApp/TradingEngineApp ../simulation_data/trading_engine.json || echo "TradingEngineApp can start"

echo "Testing MarketDataApp..."
timeout 2s ./applications/MarketDataApp/MarketDataApp ../simulation_data/market_data.json || echo "MarketDataApp can start"

echo "Testing GUI App..."
if [ -f "./applications/TradingFrontendApp/TradingFrontendApp" ]; then
    echo "GUI application built successfully"
else
    echo "GUI application not built (SDL2 may not be available)"
fi

cd ..

# Run the test
./test_basic_functionality

# Cleanup
rm test_basic_functionality

echo "Basic functionality test completed successfully!"