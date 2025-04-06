cmake_minimum_required(VERSION 3.14) 
project(Lab3 CXX)

# C++98 targets
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MainLab main_lab/main.cpp)
target_include_directories(MainLab
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/main_lab
)

enable_testing()
add_executable(LabTests main_test/tests.cpp)
target_include_directories(LabTests
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/main_test
		${CMAKE_CURRENT_SOURCE_DIR}/main_lab
)
add_test(NAME LabTests COMMAND LabTests)


# C++20 targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Main20 c20/main.cpp)
target_include_directories(Main20 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/c20
)

add_executable(LabTests20 test20/tests.cpp)
target_include_directories(LabTests20 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/test20
		${CMAKE_CURRENT_SOURCE_DIR}/c20
)
target_link_libraries(LabTests20 gtest_main)
add_test(NAME LabTests20 COMMAND LabTests20)