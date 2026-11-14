cmake_minimum_required(VERSION 3.16)
project(AstraMissionCore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(astra_core src/MissionCore.cpp)
target_include_directories(astra_core PUBLIC include)

add_executable(astra_core_tests tests/MissionCoreTests.cpp)
target_link_libraries(astra_core_tests PRIVATE astra_core)

include(CTest)
add_test(NAME astra_core_tests COMMAND astra_core_tests)
