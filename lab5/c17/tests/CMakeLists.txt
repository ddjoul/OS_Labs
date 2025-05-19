# Юнит-тесты
enable_testing()

# Тест client logic
add_executable(test_client test_client.cpp)
target_include_directories(test_client PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_link_libraries(test_client gtest gtest_main)
add_test(NAME TestClient COMMAND test_client)

# Тест server logic
add_executable(test_server test_server.cpp)
target_include_directories(test_server PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_link_libraries(test_server gtest gtest_main)
add_test(NAME TestServer COMMAND test_server)