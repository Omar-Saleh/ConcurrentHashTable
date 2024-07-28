cmake_minimum_required(VERSION 3.26)
project(ConcurrentHashTable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archiv`e/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(concurrent_hash_table_hdr HashTable.hpp)
set_target_properties(concurrent_hash_table_hdr PROPERTIES LINKER_LANGUAGE CXX)
add_executable(ConcurrentHashTable main.cpp)
target_link_libraries(ConcurrentHashTable PUBLIC concurrent_hash_table_hdr)


enable_testing()

add_executable(tester tester.cc)
target_link_libraries(tester GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tester)