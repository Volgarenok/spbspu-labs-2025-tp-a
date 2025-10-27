#include "functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

void asafov::measure_time(State &state)
{
  std::cout << "Время последнего анализа: " << state.lastDuration.count() << " секунд" << std::endl;
  state.history.push_back("measure_time");
}

void asafov::save_results(State &state, const std::string &output)
{
  std::ofstream ofs(output);
  if (!ofs)
  {
    std::cout << "Ошибка: не удалось открыть файл " << output << std::endl;
    return;
  }

  for (auto &res: state.lastResults)
  {
    ofs << "Результаты анализа текста: " << res.first << "\n";
    for (auto &p: res.second)
    {
      ofs << p.first << ": " << p.second << "\n";
    }
  }
  state.history.push_back("save_results " + output);
  std::cout << "Результаты сохранены в " << output << std::endl;
}

void asafov::clear_data(State &state)
{
  state.texts.clear();
  state.lastResults.clear();
  state.history.push_back("clear_data");
  std::cout << "Все данные очищены." << std::endl;
}

void asafov::show_history(State &state)
{
  for (size_t i = 0; i < state.history.size(); ++i)
  {
    std::cout << i + 1 << ": " << state.history[i] << std::endl;
  }
}

void asafov::clear_history(State &state)
{
  state.history.clear();
  std::cout << "История очищена." << std::endl;
}

void asafov::compare_letters(State &state, const std::string &n1, const std::string &n2)
{
  auto it1 = state.texts.find(n1);
  auto it2 = state.texts.find(n2);
  if (it1 == state.texts.end() || it2 == state.texts.end())
  {
    std::cout << "Один из текстов не найден." << std::endl;
    return;
  }

  std::map< std::string, int > f1, f2;
  for (char c: it1->second)
  {
    if (std::isalpha(static_cast< unsigned char >(c)))
    {
      std::string s(1, std::tolower(static_cast< unsigned char >(c)));
      f1[s]++;
    }
  }
  for (char c: it2->second)
  {
    if (std::isalpha(static_cast< unsigned char >(c)))
    {
      std::string s(1, std::tolower(static_cast< unsigned char >(c)));
      f2[s]++;
    }
  }

  std::cout << "Сравнение частот букв:" << std::endl;
  for (auto &p: f1)
  {
    int count2 = f2[p.first];
    std::cout << p.first << ": " << p.second << " vs " << count2 << std::endl;
  }
  state.history.push_back("compare_letters " + n1 + " " + n2);
}

void asafov::set_default_top(State &state, int N)
{
  state.defaultTop = N;
  std::cout << "Top-N по умолчанию установлен в " << N << std::endl;
  state.history.push_back("set_default_top " + std::to_string(N));
}

void asafov::show_help()
{
  std::cout << "Список команд:\n" << "loadtext <file>\n" << "analyze_letters <name>\n" << "analyze_words <name>\n" <<
      "analyze_letters_top <name> <N>\n" << "analyze_words_top <name> <N>\n" << "measure_time\n" << "save_results <file>\n" <<
      "list_texts\n" << "count_symbols <name>\n" << "count_words <name>\n" << "clear_data\n" << "delete_text <name>\n" << "show_history\n"
      << "clear_history\n" << "compare_letters <name1> <name2>\n" << "set_default_top <N>\n" << "show_help\n" << "exit\n";
}

std::vector< std::string > asafov::splitCommand(const std::string &line)
{
  std::vector< std::string > tokens;
  std::string current;
  for (char c: line)
  {
    if (c == ' ' || c == '\t')
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
    else
    {
      current.push_back(c);
    }
  }
  if (!current.empty()) tokens.push_back(current);
  return tokens;
}
