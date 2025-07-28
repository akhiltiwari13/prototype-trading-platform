#include <MarketDataProvider/LadderBuilder.hpp>
#include <MarketDataProvider/Structure.hpp>
#include <iostream>

int main() {
    using namespace MarketDataProvider;
    
    LadderBuilder builder(12345);
    
    OrderMessage order;
    order._orderId = 1.0;
    order._token = 12345;
    order._orderType = 'B';
    order._price = 100;
    order._quantity = 50;
    
    builder.processNewOrder(order);
    
    LadderDepth depth = builder.getLadderDepth();
    
    std::cout << "LadderBuilder test completed successfully!" << std::endl;
    std::cout << "Token: " << depth._token << std::endl;
    
    return 0;
}
