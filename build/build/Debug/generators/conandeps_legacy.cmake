message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(spdlog)
find_package(nlohmann_json)
find_package(Boost)
find_package(SDL2)
find_package(Tracy)
find_package(magic_enum)
find_package(yaml-cpp)
find_package(GTest)

set(CONANDEPS_LEGACY  spdlog::spdlog  nlohmann_json::nlohmann_json  boost::boost  SDL2::SDL2main  Tracy::TracyClient  magic_enum::magic_enum  yaml-cpp::yaml-cpp  gtest::gtest )