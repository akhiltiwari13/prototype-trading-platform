#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

/**
 * @brief Main GUI application for trading platform
 */
class TradingGUI {
public:
    TradingGUI();
    ~TradingGUI();

    /**
     * @brief Initialize the GUI application
     */
    bool initialize();
    
    /**
     * @brief Run the main application loop
     */
    int run();

private:
    struct OrderEntry {
        std::string symbol;
        double price = 0.0;
        int quantity = 0;
        std::string side = "BUY";
        std::string orderType = "LIMIT";
    };
    
    struct Position {
        std::string symbol;
        int quantity = 0;
        double averagePrice = 0.0;
        double currentPrice = 0.0;
        double pnl = 0.0;
    };
    
    struct OrderStatus {
        uint64_t orderId;
        std::string symbol;
        std::string side;
        double price;
        int quantity;
        int filledQuantity;
        std::string status;
        std::string timestamp;
    };

    // SDL components
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    bool _running = true;
    
    // GUI state
    OrderEntry _currentOrder;
    std::vector<Position> _positions;
    std::vector<OrderStatus> _orders;
    std::vector<std::string> _logMessages;
    
    // Network connection
    bool _connected = false;
    std::string _serverHost = "localhost";
    int _serverPort = 8080;
    
    // GUI methods
    void handleEvents();
    void render();
    void renderOrderEntry();
    void renderPositions();
    void renderOrders();
    void renderMarketData();
    void renderLogs();
    
    // Trading methods
    void placeOrder();
    void cancelOrder(uint64_t orderId);
    void connectToServer();
    void disconnectFromServer();
    
    // Utility methods
    void addLogMessage(const std::string& message);
    void updatePositions();
    void updateOrders();
    
    // Simple text rendering (placeholder - would use proper font rendering in production)
    void renderText(const std::string& text, int x, int y, SDL_Color color = {255, 255, 255, 255});
    void renderButton(const std::string& text, int x, int y, int width, int height, 
                     std::function<void()> onClick = nullptr);
    
    // Input handling
    bool _inputActive = false;
    std::string _inputBuffer;
    void handleTextInput(const std::string& text);
};

// Simple GUI components
namespace GUI {
    struct Button {
        int x, y, width, height;
        std::string text;
        std::function<void()> onClick;
        bool hovered = false;
        bool pressed = false;
    };
    
    struct InputField {
        int x, y, width, height;
        std::string* value;
        bool active = false;
        std::string placeholder;
    };
    
    struct Table {
        int x, y, width, height;
        std::vector<std::string> headers;
        std::vector<std::vector<std::string>> rows;
    };
}