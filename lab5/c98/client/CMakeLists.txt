# Клиентский модуль
add_executable(client client.cpp)

# Подключение заголовков
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/common)

# Windows-зависимости
if(WIN32)
    target_link_libraries(client Kernel32 User32)
endif()

install(TARGETS client DESTINATION bin)