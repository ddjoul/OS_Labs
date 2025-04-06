cmake_minimum_required(VERSION 3.10)
project(lab1)

set(CMAKE_TOOLCHAIN_FILE "D:/VS_libraries/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(Creator creator.cpp)
add_executable(Main main.cpp)
add_executable(Reporter reporter.cpp)

set_target_properties(Creator Main Reporter
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_executable(EmployeeTests 
    employee_test.cpp
    employee.h
)

target_link_libraries(EmployeeTests 
    GTest::GTest 
    GTest::Main
)

gtest_discover_tests(EmployeeTests)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()