#include <MarketDataProvider/MarketDataProvider.hpp>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <csignal>

/**
 * @brief Main market data application (Excalibur equivalent)
 */
class MarketDataApp {
public:
    MarketDataApp() = default;
    
    bool initialize(const std::string& configPath) {
        try {
            loadConfiguration(configPath);
            
            // Initialize stream managers
            int streamCount = _config.value("stream_count", 4);
            for (int i = 0; i < streamCount; ++i) {
                _streamManagers.emplace_back(
                    std::make_unique<MarketDataProvider::StreamManager>(1000)
                );
            }
            
            // Initialize token list from config
            MarketDataProvider::TokenListT tokenList;
            if (_config.contains("tokens")) {
                for (const auto& token : _config["tokens"]) {
                    tokenList.push_back(token.get<int>());
                }
            }
            
            // Initialize all stream managers with token list
            for (auto& manager : _streamManagers) {
                manager->init(tokenList);
            }
            
            spdlog::info("MarketDataApp initialized with {} streams and {} tokens", 
                         streamCount, tokenList.size());
            return true;
        } catch (const std::exception& e) {
            spdlog::error("Failed to initialize MarketDataApp: {}", e.what());
            return false;
        }
    }
    
    void run() {
        spdlog::info("Starting Market Data Provider...");
        
        // Start data processing threads
        for (size_t i = 0; i < _streamManagers.size(); ++i) {
            _processingThreads.emplace_back([this, i]() {
                processStream(i);
            });
        }
        
        // Main application loop
        while (_running) {
            try {
                // Monitor system health, handle recovery, etc.
                monitorSystem();
                
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } catch (const std::exception& e) {
                spdlog::error("Error in main loop: {}", e.what());
            }
        }
        
        // Wait for processing threads to finish
        for (auto& thread : _processingThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        spdlog::info("Market Data Provider stopped");
    }
    
    void stop() {
        _running = false;
    }

private:
    std::vector<std::unique_ptr<MarketDataProvider::StreamManager>> _streamManagers;
    std::vector<std::thread> _processingThreads;
    std::atomic<bool> _running{true};
    nlohmann::json _config;
    
    void loadConfiguration(const std::string& configPath) {
        std::ifstream configFile(configPath);
        if (!configFile.is_open()) {
            // Create default config if file doesn't exist
            _config = createDefaultConfig();
            std::ofstream outFile(configPath);
            outFile << _config.dump(4);
            spdlog::info("Created default configuration: {}", configPath);
            return;
        }
        
        configFile >> _config;
        spdlog::info("Configuration loaded from: {}", configPath);
    }
    
    nlohmann::json createDefaultConfig() {
        nlohmann::json config;
        config["stream_count"] = 4;
        config["host"] = "localhost";
        config["port"] = 9999;
        config["recovery_host"] = "localhost";
        config["recovery_port"] = 9998;
        config["tokens"] = nlohmann::json::array({35019, 35020, 35021, 35022});
        return config;
    }
    
    void processStream(size_t streamIndex) {
        spdlog::info("Starting stream processor {}", streamIndex);
        
        while (_running) {
            try {
                // Simulate market data processing
                // In a real implementation, this would:
                // - Receive data from network socket
                // - Parse market data messages
                // - Update order books
                // - Publish market data updates
                
                // Placeholder for actual data processing
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                
            } catch (const std::exception& e) {
                spdlog::error("Error in stream processor {}: {}", streamIndex, e.what());
            }
        }
        
        spdlog::info("Stream processor {} stopped", streamIndex);
    }
    
    void monitorSystem() {
        // Monitor system health, memory usage, connection status, etc.
        // Placeholder for system monitoring
    }
};

// Global app instance for signal handling
MarketDataApp* g_app = nullptr;

void signalHandler(int signal) {
    spdlog::info("Received signal {}, shutting down...", signal);
    if (g_app) {
        g_app->stop();
    }
}

int main(int argc, char* argv[]) {
    // Initialize logging
    spdlog::set_level(spdlog::level::info);
    spdlog::info("Starting Alternate Trading Platform - Market Data Provider");
    
    std::string configPath = "market_data.json";
    if (argc > 1) {
        configPath = argv[1];
    }
    
    try {
        MarketDataApp app;
        g_app = &app;
        
        // Setup signal handlers
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        
        if (!app.initialize(configPath)) {
            spdlog::error("Failed to initialize application");
            return 1;
        }
        
        app.run();
        
    } catch (const std::exception& e) {
        spdlog::error("Application error: {}", e.what());
        return 1;
    }
    
    spdlog::info("Market Data Provider application terminated");
    return 0;
}