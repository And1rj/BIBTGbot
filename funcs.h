#ifndef FUNCS_H
#define FUNCS_H

#include "json.hpp"
#include <cstdlib>
#include <ctime>
#include <curl/curl.h>
#include <vector>

/**
 * Возвращает текущее время в формате строки "YYYY-MM-DD HH:mm:ss"
 * @return std::string - текущее время в формате строки "YYYY-MM-DD HH:mm:ss"
 */
std::string get_time() {
  std::time_t now =
      std::time(nullptr); // получить текущее время в виде значения std::time_t
  char buf[20]; // создать буфер для отформатированной строки времени
  std::strftime(buf, sizeof(buf), "%F %T",
                std::localtime(&now)); // форматировать строку времени
  return buf; // вернуть отформатированную строку времени в виде std::string
}

/**
 * Callback-функция для libcurl, которая записывает данные в буфер std::string
 * @param buffer - буфер с данными для записи
 * @param size - размер каждого элемента данных
 * @param nmemb - количество элементов данных
 * @param html - указатель на объект std::string, в который будут записываться
 * данные
 * @return size_t - размер полученных данных
 */
static size_t Writer(char *buffer, size_t size, size_t nmemb,
                     std::string *html) {
  size_t result = size * nmemb; // вычислить размер полученных данных
  html->append(buffer, result); // добавить данные в буфер
  return result; // вернуть размер полученных данных
}

/**
 * Получает содержимое URL и возвращает его в виде std::string
 * @param url - URL для получения
 * @return std::string - содержимое URL в виде std::string
 * @throw std::runtime_error - если запрос не удался
 */
std::string curlToString(const std::string &url) {
  CURL *curl =
      curl_easy_init(); // инициализировать новый easy handle для libcurl
  std::string response; // создать std::string для хранения ответа

  if (curl) {
    char errorBuffer[CURL_ERROR_SIZE]; // создать буфер для сообщений об ошибках
                                       // libcurl
    curl_easy_setopt(curl, CURLOPT_URL,
                     url.c_str()); // установить URL для получения
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,
                     1); // следовать за HTTP-редиректами
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        Writer); // установить функцию обратного вызова для записи данных
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                     &response); // установить буфер ответа
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER,
                     errorBuffer); // установить буфер для сообщений об ошибках

    CURLcode res = curl_easy_perform(curl); // выполнить запрос
    if (res != CURLE_OK) // проверить на ошибки
    {
      throw std::runtime_error(
          std::string("curl_easy_perform() failed: ") +
          errorBuffer); // выбросить исключение с сообщением об ошибке
    }

    curl_easy_cleanup(curl); // очистить easy handle
  }

  return response; // вернуть ответ в виде std::string
}

/**
 * Получает содержимое URL и возвращает его в виде std::string
 * @param link - URL для получения
 * @return std::string - содержимое URL в виде std::string
 * @throw std::runtime_error - если запрос не удался
 */
std::string get_request(const std::string &link) {
  CURL *curl =
      curl_easy_init(); // инициализировать новый easy handle для libcurl
  std::string data; // создать std::string для хранения ответа

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     link.c_str()); // установить URL для получения
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        Writer); // установить функцию обратного вызова для записи данных
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // установить буфер ответа
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,
                     1); // следовать за HTTP-редиректами

    CURLcode res = curl_easy_perform(curl); // выполнить запрос
    if (res != CURLE_OK) // проверить на ошибки
    {
      char errorBuffer[CURL_ERROR_SIZE]; // создать буфер для сообщений об
                                         // ошибках libcurl
      curl_easy_cleanup(curl); // очистить easy handle
      throw std::runtime_error(
          std::string("curl_easy_perform() failed: ") +
          curl_easy_strerror(
              res)); // выбросить исключение с сообщением об ошибке
    }

    curl_easy_cleanup(curl); // очистить easy handle
  }

  return data; // вернуть ответ в виде std::string
}

/**
 * Конвертирует сумму из одной валюты в другую, используя курсы Центрального
 * Банка России
 * @param from - код валюты, из которой нужно конвертировать
 * @param to - код валюты, в которую нужно конвертировать
 * @param amount - сумма, которую нужно конвертировать
 * @return double - сконвертированная сумма
 * @throw std::runtime_error - если произошла ошибка при запросе курсов валют
 * или валюты не найдены
 */
double convertCurrency(const std::string &from, const std::string &to,
                       double amount) {
  std::string url = "https://www.cbr-xml-daily.ru/daily_json.js";
  std::string response = curlToString(url);
  nlohmann::json j = nlohmann::json::parse(response);

  double fromRate, toRate;

  if (from == "RUB") {
    fromRate = 1.0;
  } else {
    auto fromIt = j["Valute"].find(from);
    if (fromIt != j["Valute"].end()) {
      fromRate = fromIt.value()["Value"].get<double>() /
                 fromIt.value()["Nominal"].get<int>();
    } else {
      throw std::runtime_error("Unknown currency: " + from);
    }
  }

  if (to == "RUB") {
    toRate = 1.0;
  } else {
    auto toIt = j["Valute"].find(to);
    if (toIt != j["Valute"].end()) {
      toRate = toIt.value()["Value"].get<double>() /
               toIt.value()["Nominal"].get<int>();
    } else {
      throw std::runtime_error("Unknown currency: " + to);
    }
  }

  if (from == "RUB") {
    return amount / toRate;
  }
  if (to == "RUB") {
    return amount * fromRate;
  }
  if (from == "KZT"){
    return amount * fromRate / toRate;
  }
  if (to == "KZT"){
    return amount * fromRate / toRate ;
  }
  if (from == "UZS"){
    return amount * fromRate / toRate;
  }
  if (to == "UZS"){
    return amount * fromRate / toRate ;
  }
  if (from == "UAH"){
    return amount * fromRate / toRate;
  }
  if (to == "UAH"){
    return amount * fromRate / toRate ;
  }
  return amount * toRate / fromRate;
}

/**
 * Возвращает значение курса валюты по её коду, используя данные Центрального
 * Банка России
 * @param what - код валюты, курс которой нужно получить
 * @return float - курс валюты
 */
float get_value(char what) {
  auto js_obj = nlohmann::json::parse(
      get_request("https://www.cbr-xml-daily.ru/daily_json.js"));
  if (what == 'a') {
    return js_obj["Valute"]["USD"]["Value"].get<float>();
  }
  if (what == 'b') {
    return js_obj["Valute"]["EUR"]["Value"].get<float>();
  }
  if (what == 'c') {
    return js_obj["Valute"]["GBP"]["Value"].get<float>();
  }
  if (what == 'd') {
    return js_obj["Valute"]["BYN"]["Value"].get<float>();
  }
  if (what == 'e') {
    float uahcurr = js_obj["Valute"]["UAH"]["Value"].get<float>();
    return uahcurr / 10;
  }
  if (what == 'f') {
    return js_obj["Valute"]["CNY"]["Value"].get<float>();
  }
  if (what == 'g') {
    float kztcurr = js_obj["Valute"]["KZT"]["Value"].get<float>();
    return kztcurr / 100;
  }
  if (what == 'h') {
    float uzscurr = js_obj["Valute"]["UZS"]["Value"].get<float>();
    return uzscurr / 100;
  }
  if (what == 'i') {
    return js_obj["Valute"]["CAD"]["Value"].get<float>();
  }
  if (what == 'j') {
    return js_obj["Valute"]["JPY"]["Value"].get<float>();
  } else {
    return 0;
  }

  return -1;
}

#endif
