#define BOOST_BIND_GLOBAL_PLACEHOLDERS 
#include <stdio.h>
#include "json.hpp"
#include <cstdlib>
#include <ctime>
#include <curl/curl.h>
#include <tgbot/tgbot.h>
#include <tgbot/tools/StringTools.h>
#include <vector>
#include "funcs.h"

std::vector<std::string> bot_commands = {"start",         "time",
                                         "currency_type", "info",
                                         "converte",      "list_of_currency",
                                         "convertetion",  "conv"};

int main() {
  // Устанавливаем кодировку консоли на Windows в 1251
  system("chcp 1251>nul");

  // Создаем объект бота и передаем ему токен для доступа к API Telegram
  TgBot::Bot bot("5966035371:AAE-leTtpx8uZULfebFqaM1KZHi6H0HlsCk");

  // Создаем объект клавиатуры starting, которая будет выводиться при старте
  // бота
  TgBot::InlineKeyboardMarkup::Ptr starting(new TgBot::InlineKeyboardMarkup);

  // Создаем объекты кнопок для клавиатуры starting
  TgBot::InlineKeyboardButton::Ptr info(new TgBot::InlineKeyboardButton),
      list_of_curr(new TgBot::InlineKeyboardButton),
      currency_type(new TgBot::InlineKeyboardButton),
      time_MSK(new TgBot::InlineKeyboardButton),
      converte_f(new TgBot::InlineKeyboardButton);

  // Задаем текст и callback-данные для каждой кнопки
  info->text = "Info";
  converte_f->text = "Конвертировать валюту";
  currency_type->text = "Выбор валюты";
  time_MSK->text = "Время по Москве";
  list_of_curr->text = "Список валют";
  info->callbackData = "info";
  converte_f->callbackData = "converte";
  currency_type->callbackData = "currency_type";
  time_MSK->callbackData = "time";
  list_of_curr->callbackData = "list_of_currency";

  // Создаем векторы кнопок и добавляем в них созданные кнопки
  std::vector<TgBot::InlineKeyboardButton::Ptr> b1;
  std::vector<TgBot::InlineKeyboardButton::Ptr> b2;
  std::vector<TgBot::InlineKeyboardButton::Ptr> b3;
  b1.push_back(info);
  b1.push_back(currency_type);
  b2.push_back(time_MSK);
  b2.push_back(converte_f);
  b3.push_back(list_of_curr);

  // Добавляем векторы кнопок в объект клавиатуры starting
  starting->inlineKeyboard.push_back(b1);
  starting->inlineKeyboard.push_back(b2);
  starting->inlineKeyboard.push_back(b3);

  // Создаем объект клавиатуры keyboard, которая будет выводиться при выборе
  // команды "Выбор валюты"
  TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);

  // Создаем объекты кнопок для клавиатуры keyboard
  TgBot::InlineKeyboardButton::Ptr usd_btn(new TgBot::InlineKeyboardButton),
      eur_btn(new TgBot::InlineKeyboardButton),
      gbp_btn(new TgBot::InlineKeyboardButton),
      byn_btn(new TgBot::InlineKeyboardButton),
      uah_btn(new TgBot::InlineKeyboardButton),
      cny_btn(new TgBot::InlineKeyboardButton),
      kzt_btn(new TgBot::InlineKeyboardButton),
      uzt_btn(new TgBot::InlineKeyboardButton),
      cad_btn(new TgBot::InlineKeyboardButton),
      jpy_btn(new TgBot::InlineKeyboardButton);

  // Задаем текст и callback-данные для каждой кнопки
  usd_btn->text = "USD";
  usd_btn->callbackData = "USD";
  eur_btn->text = "EUR";
  eur_btn->callbackData = "EUR";
  gbp_btn->text = "GBP";
  gbp_btn->callbackData = "GBP";
  byn_btn->text = "BYN";
  byn_btn->callbackData = "BYN";
  uah_btn->text = "UAH";
  uah_btn->callbackData = "UAH";
  cny_btn->text = "CNY";
  cny_btn->callbackData = "CNY";
  kzt_btn->text = "KZT";
  kzt_btn->callbackData = "KZT";
  uzt_btn->text = "UZS";
  uzt_btn->callbackData = "UZS";
  cad_btn->text = "CAD";
  cad_btn->callbackData = "CAD";
  jpy_btn->text = "JPY";
  jpy_btn->callbackData = "JPY";

  // Создаем векторы кнопок и добавляем в них созданные кнопки
  std::vector<TgBot::InlineKeyboardButton::Ptr> but1;
  std::vector<TgBot::InlineKeyboardButton::Ptr> but2;
  std::vector<TgBot::InlineKeyboardButton::Ptr> but3;
  std::vector<TgBot::InlineKeyboardButton::Ptr> but4;
  std::vector<TgBot::InlineKeyboardButton::Ptr> but5;
  but1.push_back(usd_btn);
  but1.push_back(eur_btn);
  but2.push_back(gbp_btn);
  but2.push_back(byn_btn);
  but3.push_back(uah_btn);
  but3.push_back(cny_btn);
  but4.push_back(kzt_btn);
  but4.push_back(uzt_btn);
  but5.push_back(cad_btn);
  but5.push_back(jpy_btn);

  // Добавляем векторы кнопок в объект клавиатуры keyboard
  keyboard->inlineKeyboard.push_back(but1);
  keyboard->inlineKeyboard.push_back(but2);
  keyboard->inlineKeyboard.push_back(but3);
  keyboard->inlineKeyboard.push_back(but4);
  keyboard->inlineKeyboard.push_back(but5);

  /**
   * Обработчик команды /start.
   * Отправляет приветственное сообщение и клавиатуру starting.
   */
  bot.getEvents().onCommand(
      "start", [&bot, &starting](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(
            message->chat->id,
            "Здравствуйте, " + message->chat->firstName +
                ", вас приветствует Telegram Bot, который позволяет вам узнать "
                "актуальный курс на сегодняшний день.\nВот, что я умею",
            false, 0, starting);
      });

  /**
   * Обработчик команды /info.
   * Отправляет информацию о боте.
   */
  bot.getEvents().onCommand("info", [&bot](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(
        message->chat->id,
        "\tДанный бот создан студентами НИУ ВШЭ с факультета информационной "
        "безопасности в рамках дисциплины алгоритмизация и "
        "программирования.\n\t Данный бот может выводить актуальный курс "
        "валют, конвертировать их и строить графики курса.\n\t Вот полный "
        "список команд доступных вам:\n /start - команда для запуска бота\n "
        "/info - полная информация о боте\n /time - время по Москве\n "
        "/currency_type - выбор интересующей вас валюты\n /converte - "
        "конвертирует ллюбую валюту");
  });

  /**
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */
  bot.getEvents().onCallbackQuery(
      [&bot](TgBot::CallbackQuery::Ptr informationf) {
        if (informationf->data == "info") {
          bot.getApi().sendMessage(
              informationf->message->chat->id,
              "\tДанный бот создан студентами НИУ ВШЭ с факультета "
              "информационной безопасности в рамках дисциплины алгоритмизация "
              "и программирования.\n\t Данный бот может выводить актуальный "
              "курс валют, конвертировать их и строить графики курса.\n\t Вот "
              "полный список команд доступных вам:\n /start - команда для "
              "запуска бота\n /info - полная информация о боте\n /time - время "
              "по Москве\n /currency_type - выбор интересующей вас валюты\n "
              "/converte - конвертирует ллюбую валюту");
        }
      });

  /**
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */

  bot.getEvents().onCommand("time", [&bot](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(message->chat->id,
                             "Время по Москве: " + get_time());
  });
  bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr timet) {
    if (timet->data == "time") {
      bot.getApi().sendMessage(timet->message->chat->id,
                               "Время по Москве: " + get_time());
    }
  });

  /**
   * Обработчик команды /list_of_currency.
   * Отправляет сообщение со списком доступных валют.
   */
  bot.getEvents().onCommand("list_of_currency", [&bot](TgBot::Message::Ptr
                                                           message) {
    bot.getApi().sendMessage(
        message->chat->id,
        "Список валют доступных в боте:\nUSD - доллар США\nEUR - евро\nGBP - "
        "фунт стерлинга\nUAH - гривна\nBYN - белорусский рубль\nCNY - "
        "юань\nKZT - тенге\nUZT - суммы\nCAD - канадский доллар\nJPY - иены");
  });

  /*
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */
  bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr loc) {
    if (loc->data == "list_of_currency") {
      bot.getApi().sendMessage(
          loc->message->chat->id,
          "Список валют доступных в боте:\nUSD - доллар США\nEUR - евро\nGBP - "
          "фунт стерлинга\nUAH - гривна\nBYN - белорусский рубль\nCNY - "
          "юань\nKZT - тенге\nUZT - суммы\nCAD - канадский доллар\nJPY - иены");
    }
  });
  /**
   * Обработчик команды /converte.
   * Отправляет сообщение со списком доступных валют.
   */
  bot.getEvents().onCommand("converte", [&bot](TgBot::Message::Ptr message) {
    bot.getApi().sendMessage(
        message->chat->id,
        "Для конвертирования валют вам нужно написать такую команду: "
        "/conv <количество> <валюта1> <валюта2> ");
  });
  /*
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */
  bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr curr_type) {
    if (curr_type->data == "converte") {
      bot.getApi().sendMessage(
          curr_type->message->chat->id,
          "Для конвертирования валют вам нужно написать такую команду: "
          "/conv <количество> <валюта1> <валюта2> ");
    }
  });
  /*
   *Обработка команды /conv
   */
  bot.getEvents().onCommand("conv", [&bot](TgBot::Message::Ptr message) {
    std::vector<std::string> args;
    std::istringstream iss(message->text);
    std::string arg;
    while (iss >> arg) {
      args.push_back(arg);
    }
    if (args.size() != 4) {
      bot.getApi().sendMessage(
          message->chat->id,
          "Использование: /conv <количество> <валюта1> <валюта2>");
      return;
    }
    double amount;
    try {
      amount = std::stod(args[1]);
    } catch (const std::exception &e) {
      bot.getApi().sendMessage(message->chat->id,
                               "Некорректное количество валюты.");
      return;
    }

    std::string from = args[2];
    std::string to = args[3];

    try {
      double result = convertCurrency(from, to, amount);
      std::string text = std::to_string(amount) + " " + from + " = " +
                         std::to_string(result) + " " + to;
      bot.getApi().sendMessage(message->chat->id, text);
    } catch (const std::exception &e) {
      bot.getApi().sendMessage(message->chat->id, e.what());
    }
  });

  /*
   *Обработка команды /currency_type
   */
  bot.getEvents().onCommand(
      "currency_type", [&bot, &keyboard](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id,
                                 "Выберите валюту, курс которой хотите узнать ",
                                 false, 0, keyboard);
      });
  /*
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */
  bot.getEvents().onCallbackQuery([&bot, &keyboard](
                                      TgBot::CallbackQuery::Ptr curr_type) {
    if (curr_type->data == "currency_type") {
      bot.getApi().sendMessage(curr_type->message->chat->id,
                               "Выберите валюту, курс которой хотите узнать ",
                               false, 0, keyboard);
    }
  });
  /*
   * Обработчик callback-кнопок.
   * Обрабатывает нажатие кнопок на клавиатуре.
   */
  bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
    /* Если пользователь выбрал USD, отправляем сообщение с курсом */
    if (query->data == "USD") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один USD ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('a')) + " рублей");
    }
    /* Если пользователь выбрал EUR, отправляем сообщение с курсом */
    if (query->data == "EUR") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один EUR ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('b')) + " рублей");
    }
    /* Если пользователь выбрал GBP, отправляем сообщение с курсом */
    if (query->data == "GBP") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один GBP ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('c')) + " рублей");
    }
    /* Если пользователь выбрал BYN, отправляем сообщение с курсом */
    if (query->data == "BYN") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один BYN ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('d')) + " рублей");
    }
    /* Если пользователь выбрал UAH, отправляем сообщение с курсом */
    if (query->data == "UAH") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один UAH ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('e')) + " рублей");
    }
    /* Если пользователь выбрал CNY, отправляем сообщение с курсом */
    if (query->data == "CNY") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один CNY ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('f')) + " рублей");
    }
    /* Если пользователь выбрал KZT, отправляем сообщение с курсом */
    if (query->data == "KZT") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один KZT ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('g')) + " рублей");
    }
    /* Если пользователь выбрал UZS, отправляем сообщение с курсом */
    if (query->data == "UZS") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один UZS ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('h')) + " рублей");
    }
    /* Если пользователь выбрал CAD, отправляем сообщение с курсом */
    if (query->data == "CAD") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один CAD ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('i')) + " рублей");
    }
    /* Если пользователь выбрал JPY, отправляем сообщение с курсом */
    if (query->data == "JPY") {
      bot.getApi().sendMessage(query->message->chat->id,
                               "За один JPY ЦБ РФ готов вам дать: " +
                                   std::to_string(get_value('j')) + " рублей");
    }
    /* Возвращаем -1, чтобы событие не обрабатывалось дальше */
    return -1;
  });

  /* Обработка любого сообщения от пользователя */
  bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
    /* Проверяем, является ли сообщение командой, и если да, то прерываем
     * обработку */
    for (const auto &command : bot_commands) {
      if ("/" + command == message->text) {
        return;
      }
    }
  });

  try {
    /* Выводим имя бота */
    printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
    /* Запускаем бесконечный цикл для получения новых сообщений */
    TgBot::TgLongPoll longPoll(bot);
    while (true) {
      printf("Long poll started\n");
      longPoll.start();
    }
  } catch (TgBot::TgException &e) {
    /* Обработка исключений */
    printf("error: %s\n", e.what());
  }
  return 0;
};
//         g++ bibtgbot.cpp -o bibtgbot --std=c++14 -I/usr/local/incluude -lTgBot -lboost_system -lssl -lcrypto -lpthread -lcurl
//         ./bibtgbot