#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "..\c20\func.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

// Глобальные переменные для тестирования
int a;
std::atomic<int> suma;

// Тест запуска потока
TEST_CASE("Thread start") {
    a = 3;
    int b = 1;
    
    // Создаем поток с использованием std::thread и лямбды
    std::thread thread([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        suma = a + b;
    });
    
    // Ждем завершения потока
    thread.join();
    REQUIRE(suma == (a + b));
}

// Тест остановки потока
TEST_CASE("Thread stop") {
    a = 3;
    int b = 1;
    
    // Создаем поток и ждем его завершения
    std::thread thread([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        suma = a + b;
    });
    
    // В C++20 join() всегда выполняется успешно, если поток joinable
    // поэтому просто проверяем, что поток joinable перед join()
    REQUIRE(thread.joinable());
    thread.join();
    // После join() поток больше не joinable
    REQUIRE_FALSE(thread.joinable());
}

// Тест работы маркера
TEST_CASE("Thread manager works") {
    // Инициализируем ThreadManager
    int arraySize = 10;
    ThreadManager manager(arraySize);
    int threadCount = 1;
    
    // Инициализируем и создаем потоки
    manager.initialize(threadCount);
    manager.createThreads(threadCount);
    
    // Ожидаем, пока все потоки остановятся
    manager.waitForAllThreadsToStop();
    REQUIRE(manager.areAllThreadsStopped());
    
    // Удаляем поток
    int threadId = 0;
    manager.deleteThread(threadId);
    
    // Проверяем, что количество потоков стало 0
    REQUIRE(manager.getThreadCount() == 0);
    
    // Проверяем, что поток больше не работает
    REQUIRE_FALSE(manager.isThreadWorkingAt(threadId));
}

// Дополнительный тест для проверки маркировки элементов
TEST_CASE("Thread marking elements") {
    // Создаем менеджер с массивом из 5 элементов и 1 потоком
    ThreadManager manager(5);
    manager.initialize(1);
    manager.createThreads(1);
    
    // Даем потоку время на маркировку элементов
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Ждем остановки потока
    manager.waitForAllThreadsToStop();
    REQUIRE(manager.areAllThreadsStopped());
    
    // Удаляем поток, что должно очистить помеченные элементы
    manager.deleteThread(0);
    REQUIRE(manager.getThreadCount() == 0);
}

// Тест для проверки возобновления работы потоков
TEST_CASE("Thread resume") {
    ThreadManager manager(5);
    manager.initialize(1);
    manager.createThreads(1);
    
    // Даем потоку время остановиться
    manager.waitForAllThreadsToStop();
    REQUIRE(manager.areAllThreadsStopped());
    
    // Возобновляем работу потока
    manager.resumeAllThreads();
    
    // Даем потоку время снова остановиться
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    manager.waitForAllThreadsToStop();
    REQUIRE(manager.areAllThreadsStopped());
}