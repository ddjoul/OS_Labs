cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Creator creator.cpp)
add_executable(Main main.cpp)
add_executable(Reporter reporter.cpp)

set_target_properties(Creator Main Reporter
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Use static runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
