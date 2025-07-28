#pragma once

#include <memory>
#include <string>
#include <functional>
#include <thread>
#include <atomic>

namespace TradingEngine {

/**
 * @brief Client connection handler
 */
class ClientConnection {
public:
    virtual ~ClientConnection() = default;
    
    /**
     * @brief Send data to client
     */
    virtual bool send(const std::string& data) = 0;
    
    /**
     * @brief Close connection
     */
    virtual void close() = 0;
    
    /**
     * @brief Get client identifier
     */
    virtual std::string getClientId() const = 0;
};

using ClientConnectionPtr = std::shared_ptr<ClientConnection>;
using MessageHandler = std::function<void(ClientConnectionPtr, const std::string&)>;
using ConnectionHandler = std::function<void(ClientConnectionPtr)>;

/**
 * @brief Socket server for client connections
 */
class SocketServer {
public:
    SocketServer(int port);
    ~SocketServer();

    /**
     * @brief Start the server
     */
    bool start();
    
    /**
     * @brief Stop the server
     */
    void stop();
    
    /**
     * @brief Set message handler
     */
    void setMessageHandler(MessageHandler handler);
    
    /**
     * @brief Set connection handler
     */
    void setConnectionHandler(ConnectionHandler connectHandler, ConnectionHandler disconnectHandler);
    
    /**
     * @brief Broadcast message to all clients
     */
    void broadcast(const std::string& message);
    
    /**
     * @brief Send message to specific client
     */
    bool sendToClient(const std::string& clientId, const std::string& message);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace TradingEngine