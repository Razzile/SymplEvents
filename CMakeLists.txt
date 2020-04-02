cmake_minimum_required(VERSION 3.10)
 
project(EventManager C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("third_party/Catch2")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/third_party/Catch2/contrib/")
include(CTest)
include(Catch)

include_directories("src")

add_executable(test_delegate "tests/test_delegate.cc")
add_executable(test_events "tests/test_events.cc")
add_executable(example "tests/example.cc")


target_link_libraries(test_delegate Catch2::Catch2)
target_link_libraries(test_events Catch2::Catch2)

catch_discover_tests(test_delegate)
catch_discover_tests(test_events)