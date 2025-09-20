#include <iostream>
#include <string>
#include <vector>
#include "functions.h"

#ifdef _WIN32 ||_WIN64 // для русского языка в винде
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32 ||_WIN64 // тут тоже
  SetConsoleOutputCP(CP_UTF8);
#endif

  asafov::State state;

  std::cout << "Частотный анализ текста. Введите команду (show_help для справки)." << std::endl;

  std::string line;
  while (true)
  {
    std::cout << "> ";
    if (!std::getline(std::cin, line)) break;

    auto tokens = asafov::splitCommand(line);
    if (tokens.empty()) continue;

    const std::string &cmd = tokens[0];

    if (cmd == "exit") break;
    else if (cmd == "show_help") asafov::show_help();
    else if (cmd == "load_text" && tokens.size() > 1) asafov::loadtext(state, tokens[1]);
    else if (cmd == "analyze_letters" && tokens.size() > 1) asafov::analyze_letters(state, tokens[1]);
    else if (cmd == "analyze_words" && tokens.size() > 1) asafov::analyze_words(state, tokens[1]);
    else if (cmd == "analyze_letters_top" && tokens.size() > 2) asafov::analyze_letters_top(state, tokens[1], std::stoi(tokens[2]));
    else if (cmd == "analyze_words_top" && tokens.size() > 2) asafov::analyze_words_top(state, tokens[1], std::stoi(tokens[2]));
    else if (cmd == "measure_time") asafov::measure_time(state);
    else if (cmd == "save_results" && tokens.size() > 1) asafov::save_results(state, tokens[1]);
    else if (cmd == "list_texts") asafov::list_texts(state);
    else if (cmd == "count_symbols" && tokens.size() > 1) asafov::count_symbols(state, tokens[1]);
    else if (cmd == "count_words" && tokens.size() > 1) asafov::count_words(state, tokens[1]);
    else if (cmd == "clear_data") asafov::clear_data(state);
    else if (cmd == "delete_text" && tokens.size() > 1) asafov::delete_text(state, tokens[1]);
    else if (cmd == "show_history") asafov::show_history(state);
    else if (cmd == "clear_history") asafov::clear_history(state);
    else if (cmd == "compare_letters" && tokens.size() > 2) asafov::compare_letters(state, tokens[1], tokens[2]);
    else if (cmd == "set_default_top" && tokens.size() > 1) asafov::set_default_top(state, std::stoi(tokens[1]));
    else std::cout << "Неизвестная команда или неверные параметры." << std::endl;
  }
  return 0;
}
