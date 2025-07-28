#!/bin/bash

# Run the trading platform in simulation mode

set -e

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check if build exists
if [ ! -d "build" ]; then
    print_info "Build directory not found. Building project..."
    ./fix_build.sh
fi

# Create simulation data directory
mkdir -p simulation_data

# Create sample database
print_info "Creating sample database..."

# Check if sample database creator exists in build
if [ ! -f "build/simulation/SampleDataCreator" ]; then
    print_info "Building sample database creator..."
    
    # Rebuild to include the new target
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    cmake --build . --target SampleDataCreator
    cd ..
fi

# Run the sample data creator
./build/simulation/SampleDataCreator

# Create configuration files
print_info "Creating configuration files..."

# Trading engine config
cat > simulation_data/trading_engine.json << 'EOF'
{
  "server": {
    "port": 8080,
    "max_connections": 1000
  },
  "database": {
    "path": "simulation_data/contracts.db",
    "connection_pool_size": 10
  },
  "risk_management": {
    "max_order_value": 1000000.0,
    "max_order_quantity": 10000,
    "max_daily_loss": 50000.0,
    "max_position_value": 500000.0,
    "max_orders_per_second": 10
  },
  "simulation": {
    "enabled": true,
    "auto_fill_orders": true,
    "fill_delay_ms": 100
  }
}
EOF

# Market data config
cat > simulation_data/market_data.json << 'EOF'
{
  "stream_count": 4,
  "host": "localhost",
  "port": 9999,
  "recovery_host": "localhost",
  "recovery_port": 9998,
  "tokens": [35019, 35020, 35021, 35022, 35023, 36690, 36691, 36692, 36693, 36694],
  "simulation": {
    "enabled": true,
    "base_price": 18500.0,
    "volatility": 0.02,
    "ticks_per_second": 10,
    "max_orders_per_tick": 5,
    "enable_trades": true
  }
}
EOF

print_success "Configuration files created"

# Function to start applications
start_market_data() {
    print_info "Starting Market Data Provider..."
    cd build
    ./applications/MarketDataApp/MarketDataApp ../simulation_data/market_data.json &
    MARKET_DATA_PID=$!
    cd ..
    sleep 2
}

start_trading_engine() {
    print_info "Starting Trading Engine..."
    cd build
    ./applications/TradingEngineApp/TradingEngineApp ../simulation_data/trading_engine.json &
    TRADING_ENGINE_PID=$!
    cd ..
    sleep 2
}

start_gui() {
    print_info "Starting Trading GUI..."
    cd build
    if [ -f "applications/TradingFrontendApp/TradingFrontendApp" ]; then
        ./applications/TradingFrontendApp/TradingFrontendApp &
        GUI_PID=$!
    else
        print_warning "GUI application not built (SDL2 may not be available)"
    fi
    cd ..
}

# Cleanup function
cleanup() {
    print_info "Shutting down applications..."
    if [ ! -z "$MARKET_DATA_PID" ]; then
        kill $MARKET_DATA_PID 2>/dev/null || true
    fi
    if [ ! -z "$TRADING_ENGINE_PID" ]; then
        kill $TRADING_ENGINE_PID 2>/dev/null || true
    fi
    if [ ! -z "$GUI_PID" ]; then
        kill $GUI_PID 2>/dev/null || true
    fi
    print_success "Cleanup completed"
}

# Set up signal handlers
trap cleanup EXIT INT TERM

# Start applications
print_info "Starting Alternate Trading Platform in simulation mode..."

start_market_data
start_trading_engine
start_gui

print_success "All applications started!"
print_info "Market Data Provider PID: $MARKET_DATA_PID"
print_info "Trading Engine PID: $TRADING_ENGINE_PID"
if [ ! -z "$GUI_PID" ]; then
    print_info "GUI Application PID: $GUI_PID"
fi

print_info "Press Ctrl+C to stop all applications"

# Wait for user input or process termination
wait