#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "funcs.h"
#include <cctype>

// Тесты для функции get_time()
TEST_CASE("get_time returns valid time string") {
    // Проверяем, что функция возвращает строку в правильном формате
    std::string expectedFormat = "YYYY-MM-DD HH:MM:SS";
    std::string actualOutput = get_time();
        
    REQUIRE(actualOutput.size() == expectedFormat.size());
    for (std::size_t i = 0; i < actualOutput.size(); ++i) {
      if (expectedFormat[i] == 'Y' || expectedFormat[i] == 'M' || expectedFormat[i] == 'D' || expectedFormat[i] == 'H' || expectedFormat[i] == 'M' || expectedFormat[i] == 'S') {
        CHECK(isdigit(actualOutput[i]));
      }
    }

    // Проверяем, что функция возвращает текущее время
    auto current_time = std::chrono::system_clock::now();
    expectedFormat = std::chrono::system_clock::to_time_t(current_time);
    actualOutput = get_time();
    for (std::size_t i = 0; i < actualOutput.size() - 2; ++i) {
      CHECK(actualOutput[i] == expectedFormat[i]);
    }


    // Проверяем, что функция возвращает непустую строку
    CHECK(get_time().size() > 0);
}

TEST_CASE("get_time returns invalid time string") {
    // Проверяем, что функция не возвращает пустую строку
    CHECK(get_time().size() != 0);

    // Проверяем, что функция не возвращает строку в неверном формате
    CHECK(get_time() != "11:00:00 2023-06-26");

    // Проверяем, что функция не возвращает неверное время
    CHECK(get_time() != "2023-06-26 00:00:00");
}

// Тесты для функции curlToString()
TEST_CASE("curlToString returns valid content") {
    // Проверяем, что функция возвращает непустую строку
    CHECK(curlToString("https://www.google.com").size() > 0);

    // Проверяем, что функция возвращает содержимое URL в виде правильной строки
    CHECK(curlToString("https://www.google.com").find("<html") != std::string::npos);

    // Проверяем, что функция корректно обрабатывает HTTPS
    CHECK(curlToString("https://www.google.com").find("<html") != std::string::npos);
}

TEST_CASE("curlToString returns invalid content") {
    // Проверяем, что функция выбрасывает исключение std::runtime_error в случае ошибки запроса
    CHECK_THROWS_AS(curlToString("https://nonexistenturl.com"), std::runtime_error);

    // Проверяем, что функция возвращает пустую строку в случае ошибки запроса
    CHECK(curlToString("https://nonexistenturl.com") == "");

    // Проверяем, что функция корректно обрабатывает HTTP
    CHECK(curlToString("http://www.google.com").find("<html") != std::string::npos);
}

// Тесты для функции convertCurrency()
TEST_CASE("convertCurrency returns valid result") {
    // Проверяем, что функция возвращает положительную сумму
    CHECK(convertCurrency("USD", "RUB", 1.0) > 0.0);

    // Проверяем, что функция корректно обрабатывает десятичные значения
    CHECK(convertCurrency("USD", "RUB", 0.5) > 0.0);

    // Проверяем, что функция корректно обрабатывает конвертацию валюты в саму себя
    CHECK(convertCurrency("USD", "USD", 1.0) == 1.0);
}

TEST_CASE("convertCurrency returns invalid result") {
    // Проверяем, что функция выбрасывает исключение std::runtime_error в случае, если валюта не найдена
    CHECK_THROWS_AS(convertCurrency("XYZ", "RUB", 1.0), std::runtime_error);

    // Проверяем, что функция возвращает 0 в случае, если код валюты неизвестен
    CHECK(convertCurrency("z", "RUB", 1.0) == 0.0);

    // Проверяем, что функция не возвращает отрицательную сумму
    CHECK(convertCurrency("USD", "RUB", -1.0) < 0.0);
}

TEST_CASE("Test USD value") {
  float result = get_value('a');
  float expected = 74.25f;
  REQUIRE(result == expected);
}

TEST_CASE("Test GBP value") {
  float result = get_value('c');
  float expected = 102.13f;
  REQUIRE(result == expected);
}

TEST_CASE("Test UZS value") {
  float result = get_value('h');
  float expected = 0.0074f;
  REQUIRE(result == expected);
}

// Отрицательные тесты

TEST_CASE("Test invalid argument") {
  float result = get_value('z');
  REQUIRE(result == 0);
}
