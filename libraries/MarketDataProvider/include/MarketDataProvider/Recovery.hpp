#pragma once

#include "MarketDataProvider/Structure.hpp"
#include <memory>
#include <string>

namespace MarketDataProvider {

/**
 * @brief Market data recovery mechanism
 */
class Recovery {
public:
    Recovery(const std::string& host_, int port_);
    ~Recovery();

    /**
     * @brief Request recovery for missing sequence numbers
     */
    bool requestRecovery(short streamId_, int startSeq_, int endSeq_);
    
    /**
     * @brief Process recovery response
     */
    void processRecoveryResponse(const RecoveryResponse& response_);
    
    /**
     * @brief Check if recovery is needed
     */
    bool isRecoveryNeeded(short streamId_, int expectedSeq_, int receivedSeq_);

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};

} // namespace MarketDataProvider