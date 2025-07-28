#include "TradingGUI.hpp"
#include <spdlog/spdlog.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialize logging
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Starting Alternate Trading Platform - GUI Frontend");
    
    try {
        TradingGUI app;
        
        if (!app.initialize()) {
            spdlog::error("Failed to initialize GUI application");
            return 1;
        }
        
        spdlog::info("GUI application initialized successfully");
        
        // Run the application
        return app.run();
        
    } catch (const std::exception& e) {
        spdlog::error("GUI application error: {}", e.what());
        return 1;
    }
    
    return 0;
}