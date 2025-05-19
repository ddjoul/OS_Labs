# Серверный модуль
add_executable(server server.cpp)

# Подключение заголовков из common
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/common)

# Специфичные для Windows настройки
if(WIN32)
    target_link_libraries(server Kernel32 User32)
endif()

# Установка в папку bin
install(TARGETS server DESTINATION bin)