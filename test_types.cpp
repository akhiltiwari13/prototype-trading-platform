#include <MarketDataProvider/Structure.hpp>
#include <iostream>

int main() {
    using namespace MarketDataProvider;
    
    // Test OrderContainerT
    OrderContainerT orderBook;
    
    // Test insertion
    Order order{100, 50};
    orderBook[1.0] = order;
    
    // Test iteration
    for (auto it = orderBook.begin(); it != orderBook.end(); ++it) {
        std::cout << "Order ID: " << it->first << ", Price: " << it->second._price 
                  << ", Quantity: " << it->second._quantity << std::endl;
    }
    
    // Test find
    auto it = orderBook.find(1.0);
    if (it != orderBook.end()) {
        Order& foundOrder = it->second;
        std::cout << "Found order - Price: " << foundOrder._price 
                  << ", Quantity: " << foundOrder._quantity << std::endl;
    }
    
    // Test ContainerT (ladder)
    ContainerT<std::less<PriceT>> bidLadder;
    bidLadder[100] = 50;
    
    std::cout << "Type test completed successfully!" << std::endl;
    return 0;
}