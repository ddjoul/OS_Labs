cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

if(WIN32)
    add_executable(winapi_tests 
        ${CMAKE_CURRENT_SOURCE_DIR}/winapi/winapi_tests.cpp 
        ${CMAKE_SOURCE_DIR}/src/winapi/thread_processor.cpp)
    target_link_libraries(winapi_tests PRIVATE gtest_main gmock)
    
    add_test(NAME winapi_tests COMMAND winapi_tests)
endif()

add_executable(stdthread_tests 
    ${CMAKE_CURRENT_SOURCE_DIR}/stdthread/stdthread_tests.cpp 
    ${CMAKE_SOURCE_DIR}/src/stdthread/thread_processor.cpp)
target_link_libraries(stdthread_tests PRIVATE gtest_main gmock Threads::Threads)

set_target_properties(stdthread_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

add_test(NAME stdthread_tests 
         COMMAND ${CMAKE_BINARY_DIR}/tests/stdthread_tests)