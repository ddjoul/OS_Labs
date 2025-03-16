cmake_minimum_required(VERSION 3.14)
project(thread_array_processor VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the Threads package
find_package(Threads REQUIRED)

# Option to build tests
option(BUILD_TESTS "Build the tests" ON)

# Windows-specific configuration
if(WIN32)
    # Source files for WinAPI implementation
    set(WINAPI_SOURCES
        src/winapi/thread_processor.cpp
        src/winapi/main.cpp
    )
    
    # Create WinAPI executable
    add_executable(winapi_processor ${WINAPI_SOURCES})
    
    # Add Windows-specific libraries
    target_link_libraries(winapi_processor PRIVATE)
endif()

# Standard thread implementation (works on all platforms)
set(STDTHREAD_SOURCES
    src/stdthread/thread_processor.cpp
    src/stdthread/main.cpp
)

# Create std::thread executable
add_executable(stdthread_processor ${STDTHREAD_SOURCES})

# Add thread library for std::thread implementation
find_package(Threads REQUIRED)
target_link_libraries(stdthread_processor PRIVATE Threads::Threads)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()