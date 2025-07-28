#!/bin/bash

# Test the fixes we just applied

set -e

echo "Testing fixes..."

# Build the project
echo "Building project..."
./fix_build.sh

echo "Running individual tests to verify fixes..."

cd build

# Test OptionsGreeks (should pass now)
echo "Testing OptionsGreeks..."
if ./tests/libraries/OptionsGreeks/OptionsGreeksTests; then
    echo "✅ OptionsGreeks tests PASSED"
else
    echo "❌ OptionsGreeks tests still failing"
fi

# Test TradingEngine (should pass now)
echo "Testing TradingEngine..."
if ./tests/libraries/TradingEngine/TradingEngineTests; then
    echo "✅ TradingEngine tests PASSED"
else
    echo "❌ TradingEngine tests still failing"
fi

cd ..

# Test basic functionality
echo "Testing basic functionality..."
if ./test_basic.sh; then
    echo "✅ Basic functionality test PASSED"
else
    echo "❌ Basic functionality test still failing"
fi

echo "Fix testing completed!"