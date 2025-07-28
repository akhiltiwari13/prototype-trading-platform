#pragma once

#include <memory>
#include <string>
#include <functional>

namespace MarketDataProvider {

/**
 * @brief Network socket for market data reception
 */
class NetworkSocket {
public:
    using DataCallback = std::function<void(const char*, size_t)>;
    
    NetworkSocket(const std::string& host_, int port_);
    ~NetworkSocket();

    /**
     * @brief Connect to market data server
     */
    bool connect();
    
    /**
     * @brief Disconnect from server
     */
    void disconnect();
    
    /**
     * @brief Start receiving data
     */
    void startReceiving(DataCallback callback_);
    
    /**
     * @brief Stop receiving data
     */
    void stopReceiving();
    
    /**
     * @brief Send data to server
     */
    bool send(const char* data_, size_t size_);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace MarketDataProvider