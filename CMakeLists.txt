cmake_minimum_required(VERSION 3.16)
project(NovaFrontierCore VERSION 0.1.0 LANGUAGES CXX)

add_library(nova_frontier_core Source/Core/GameRules.cpp)
target_include_directories(nova_frontier_core PUBLIC Source/Core)
target_compile_features(nova_frontier_core PUBLIC cxx_std_17)

add_executable(nova_frontier_core_tests Tests/GameRulesTests.cpp)
target_link_libraries(nova_frontier_core_tests PRIVATE nova_frontier_core)

enable_testing()
add_test(NAME nova_frontier_core_tests COMMAND nova_frontier_core_tests)
