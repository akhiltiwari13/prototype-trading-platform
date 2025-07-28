#include "TradingGUI.hpp"
#include <spdlog/spdlog.h>
#include <chrono>
#include <iomanip>
#include <sstream>

TradingGUI::TradingGUI() {
    spdlog::info("TradingGUI constructor");
}

TradingGUI::~TradingGUI() {
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
    }
    if (_window) {
        SDL_DestroyWindow(_window);
    }
    SDL_Quit();
}

bool TradingGUI::initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        spdlog::error("SDL initialization failed: {}", SDL_GetError());
        return false;
    }
    
    // Create window
    _window = SDL_CreateWindow(
        "Alternate Trading Platform",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1200, 800,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!_window) {
        spdlog::error("Window creation failed: {}", SDL_GetError());
        return false;
    }
    
    // Create renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        spdlog::error("Renderer creation failed: {}", SDL_GetError());
        return false;
    }
    
    // Initialize sample data
    _positions = {
        {"NIFTY24JAN18500CE", 50, 125.50, 130.25, 237.50},
        {"BANKNIFTY24JAN45000PE", -25, 180.75, 175.25, 137.50},
        {"RELIANCE", 100, 2450.25, 2465.50, 1525.00}
    };
    
    _orders = {
        {1001, "NIFTY24JAN18500CE", "BUY", 125.50, 50, 50, "FILLED", "10:30:15"},
        {1002, "BANKNIFTY24JAN45000PE", "SELL", 180.75, 25, 25, "FILLED", "10:32:22"},
        {1003, "RELIANCE", "BUY", 2465.00, 50, 0, "PENDING", "10:35:10"}
    };
    
    addLogMessage("GUI initialized successfully");
    addLogMessage("Ready for trading");
    
    return true;
}

int TradingGUI::run() {
    while (_running) {
        handleEvents();
        render();
        
        // Cap frame rate
        SDL_Delay(16); // ~60 FPS
    }
    
    return 0;
}

void TradingGUI::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _running = false;
                break;
                
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    _running = false;
                }
                break;
                
            case SDL_TEXTINPUT:
                if (_inputActive) {
                    handleTextInput(event.text.text);
                }
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                // Handle button clicks (simplified)
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    
                    // Check if clicking on place order button (approximate position)
                    if (mouseX >= 50 && mouseX <= 150 && mouseY >= 200 && mouseY <= 230) {
                        placeOrder();
                    }
                    
                    // Check if clicking on connect button
                    if (mouseX >= 200 && mouseX <= 300 && mouseY >= 200 && mouseY <= 230) {
                        if (_connected) {
                            disconnectFromServer();
                        } else {
                            connectToServer();
                        }
                    }
                }
                break;
        }
    }
}

void TradingGUI::render() {
    // Clear screen
    SDL_SetRenderDrawColor(_renderer, 20, 20, 30, 255);
    SDL_RenderClear(_renderer);
    
    // Render different sections
    renderOrderEntry();
    renderPositions();
    renderOrders();
    renderMarketData();
    renderLogs();
    
    // Present the rendered frame
    SDL_RenderPresent(_renderer);
}

void TradingGUI::renderOrderEntry() {
    // Title
    renderText("Order Entry", 20, 20, {255, 255, 255, 255});
    
    // Connection status
    std::string connectionStatus = _connected ? "Connected" : "Disconnected";
    SDL_Color statusColor = _connected ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 0, 0, 255};
    renderText("Status: " + connectionStatus, 20, 45, statusColor);
    
    // Order form (simplified)
    renderText("Symbol: " + _currentOrder.symbol, 20, 80);
    renderText("Price: " + std::to_string(_currentOrder.price), 20, 105);
    renderText("Quantity: " + std::to_string(_currentOrder.quantity), 20, 130);
    renderText("Side: " + _currentOrder.side, 20, 155);
    
    // Buttons
    renderButton("Place Order", 50, 200, 100, 30, [this]() { placeOrder(); });
    renderButton(_connected ? "Disconnect" : "Connect", 200, 200, 100, 30, 
                [this]() { 
                    if (_connected) disconnectFromServer(); 
                    else connectToServer(); 
                });
}

void TradingGUI::renderPositions() {
    renderText("Positions", 400, 20, {255, 255, 255, 255});
    
    int y = 50;
    renderText("Symbol          Qty    Avg Price   Current    P&L", 400, y);
    y += 25;
    
    for (const auto& pos : _positions) {
        std::stringstream ss;
        ss << std::left << std::setw(15) << pos.symbol
           << std::setw(7) << pos.quantity
           << std::setw(10) << std::fixed << std::setprecision(2) << pos.averagePrice
           << std::setw(10) << pos.currentPrice
           << std::setw(10) << pos.pnl;
        
        SDL_Color color = pos.pnl >= 0 ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 0, 0, 255};
        renderText(ss.str(), 400, y, color);
        y += 20;
    }
}

void TradingGUI::renderOrders() {
    renderText("Orders", 400, 250, {255, 255, 255, 255});
    
    int y = 280;
    renderText("ID    Symbol          Side  Price    Qty  Filled  Status    Time", 400, y);
    y += 25;
    
    for (const auto& order : _orders) {
        std::stringstream ss;
        ss << std::left << std::setw(6) << order.orderId
           << std::setw(15) << order.symbol
           << std::setw(6) << order.side
           << std::setw(9) << std::fixed << std::setprecision(2) << order.price
           << std::setw(5) << order.quantity
           << std::setw(8) << order.filledQuantity
           << std::setw(10) << order.status
           << order.timestamp;
        
        SDL_Color color = {200, 200, 200, 255};
        if (order.status == "FILLED") color = {0, 255, 0, 255};
        else if (order.status == "CANCELLED") color = {255, 0, 0, 255};
        else if (order.status == "PENDING") color = {255, 255, 0, 255};
        
        renderText(ss.str(), 400, y, color);
        y += 20;
    }
}

void TradingGUI::renderMarketData() {
    renderText("Market Data", 20, 300, {255, 255, 255, 255});
    
    // Simplified market data display
    int y = 330;
    renderText("NIFTY: 18,525.50 (+15.25)", 20, y, {0, 255, 0, 255});
    y += 20;
    renderText("BANKNIFTY: 45,125.75 (-25.50)", 20, y, {255, 0, 0, 255});
    y += 20;
    renderText("RELIANCE: 2,465.50 (+12.25)", 20, y, {0, 255, 0, 255});
}

void TradingGUI::renderLogs() {
    renderText("Logs", 20, 500, {255, 255, 255, 255});
    
    int y = 530;
    int maxLogs = 10;
    int startIndex = std::max(0, static_cast<int>(_logMessages.size()) - maxLogs);
    
    for (int i = startIndex; i < _logMessages.size(); ++i) {
        renderText(_logMessages[i], 20, y, {180, 180, 180, 255});
        y += 15;
    }
}

void TradingGUI::placeOrder() {
    // Simulate order placement
    static uint64_t nextOrderId = 1004;
    
    if (_currentOrder.symbol.empty() || _currentOrder.quantity <= 0) {
        addLogMessage("Invalid order parameters");
        return;
    }
    
    OrderStatus newOrder;
    newOrder.orderId = nextOrderId++;
    newOrder.symbol = _currentOrder.symbol.empty() ? "NIFTY24JAN18500CE" : _currentOrder.symbol;
    newOrder.side = _currentOrder.side;
    newOrder.price = _currentOrder.price > 0 ? _currentOrder.price : 125.50;
    newOrder.quantity = _currentOrder.quantity > 0 ? _currentOrder.quantity : 50;
    newOrder.filledQuantity = 0;
    newOrder.status = "PENDING";
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream timeStr;
    timeStr << std::put_time(&tm, "%H:%M:%S");
    newOrder.timestamp = timeStr.str();
    
    _orders.push_back(newOrder);
    
    addLogMessage("Order placed: " + newOrder.symbol + " " + newOrder.side + 
                  " " + std::to_string(newOrder.quantity) + " @ " + std::to_string(newOrder.price));
}

void TradingGUI::cancelOrder(uint64_t orderId) {
    for (auto& order : _orders) {
        if (order.orderId == orderId && order.status == "PENDING") {
            order.status = "CANCELLED";
            addLogMessage("Order cancelled: " + std::to_string(orderId));
            break;
        }
    }
}

void TradingGUI::connectToServer() {
    // Simulate connection
    _connected = true;
    addLogMessage("Connected to trading server at " + _serverHost + ":" + std::to_string(_serverPort));
}

void TradingGUI::disconnectFromServer() {
    _connected = false;
    addLogMessage("Disconnected from trading server");
}

void TradingGUI::addLogMessage(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream timeStr;
    timeStr << std::put_time(&tm, "%H:%M:%S");
    
    _logMessages.push_back("[" + timeStr.str() + "] " + message);
    
    // Keep only last 100 messages
    if (_logMessages.size() > 100) {
        _logMessages.erase(_logMessages.begin());
    }
}

void TradingGUI::updatePositions() {
    // Simulate position updates
    for (auto& pos : _positions) {
        // Add some random price movement
        double change = (rand() % 200 - 100) / 100.0; // -1.00 to +1.00
        pos.currentPrice += change;
        pos.pnl = pos.quantity * (pos.currentPrice - pos.averagePrice);
    }
}

void TradingGUI::updateOrders() {
    // Simulate order fills
    for (auto& order : _orders) {
        if (order.status == "PENDING" && rand() % 100 < 5) { // 5% chance per frame
            order.filledQuantity = order.quantity;
            order.status = "FILLED";
            addLogMessage("Order filled: " + std::to_string(order.orderId));
        }
    }
}

void TradingGUI::renderText(const std::string& text, int x, int y, SDL_Color color) {
    // This is a placeholder for text rendering
    // In a real implementation, you would use a font library like SDL_ttf
    // For now, we'll just draw colored rectangles to represent text
    
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, static_cast<int>(text.length() * 8), 12};
    SDL_RenderDrawRect(_renderer, &rect);
}

void TradingGUI::renderButton(const std::string& text, int x, int y, int width, int height, 
                             std::function<void()> onClick) {
    // Draw button background
    SDL_SetRenderDrawColor(_renderer, 60, 60, 80, 255);
    SDL_Rect buttonRect = {x, y, width, height};
    SDL_RenderFillRect(_renderer, &buttonRect);
    
    // Draw button border
    SDL_SetRenderDrawColor(_renderer, 100, 100, 120, 255);
    SDL_RenderDrawRect(_renderer, &buttonRect);
    
    // Draw button text (placeholder)
    renderText(text, x + 5, y + 5, {255, 255, 255, 255});
}

void TradingGUI::handleTextInput(const std::string& text) {
    if (_inputActive) {
        _inputBuffer += text;
    }
}