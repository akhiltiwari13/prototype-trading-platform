#!/bin/bash

# Quick test build for MarketDataProvider

set -e

echo "Testing MarketDataProvider build fix..."

cd build

# Build just the MarketDataProvider target
echo "Building MarketDataProvider..."
cmake --build . --target MarketDataProvider

echo "MarketDataProvider build completed successfully!"