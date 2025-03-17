cmake_minimum_required(VERSION 3.14)
project(thread_array_processor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

option(BUILD_TESTS "Build the tests" ON)

if(WIN32)
    set(WINAPI_SOURCES
        src/winapi/thread_processor.cpp
        src/winapi/main.cpp
    )
    
    add_executable(winapi_processor ${WINAPI_SOURCES})
    
    target_link_libraries(winapi_processor PRIVATE)
endif()

set(STDTHREAD_SOURCES
    src/stdthread/thread_processor.cpp
    src/stdthread/main.cpp
)

add_executable(stdthread_processor ${STDTHREAD_SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(stdthread_processor PRIVATE Threads::Threads)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()