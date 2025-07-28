#include "SampleData.hpp"
#include <iostream>

int main() {
    try {
        Simulation::SampleData::createSampleDatabase("simulation_data/contracts.db");
        std::cout << "Sample database created successfully" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}