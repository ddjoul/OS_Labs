cmake_minimum_required(VERSION 2.8)
project(EmployeeAccessSystem)
include(FetchContent)
# Общие настройки
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
endif()

# Подключение поддиректорий
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)