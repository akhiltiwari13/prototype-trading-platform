cmake_minimum_required(VERSION 3.15)

# Create sample data creator executable
add_executable(SampleDataCreator create_sample_db.cpp SampleData.cpp)
target_link_libraries(SampleDataCreator DatabaseLayer nlohmann_json::nlohmann_json)
target_include_directories(SampleDataCreator PRIVATE ../libraries/DatabaseLayer/include)