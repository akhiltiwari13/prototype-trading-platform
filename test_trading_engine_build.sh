#!/bin/bash

# Quick test for TradingEngine build

set -e

echo "Testing TradingEngine build..."

cd build

# Build just the TradingEngine target
echo "Building TradingEngine..."
cmake --build . --target TradingEngine

echo "TradingEngine build completed successfully!"