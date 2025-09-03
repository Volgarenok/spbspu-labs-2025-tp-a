#include "functions.h"
#include <map>
#include <cctype>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <numeric>
#include <iterator>
#include <algorithm>

namespace
{
  std::map<std::string, asafov::TextData> texts;
  int default_top = 10;
  std::chrono::steady_clock::time_point last_analysis_start;
  std::chrono::steady_clock::time_point last_analysis_end;

  void load_text(const std::string& name, const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file)
    {
      std::cerr << "Ошибка: не удалось открыть файл " << filename << '\n';
      return;
    }

    asafov::TextData data;
    std::copy(std::istreambuf_iterator<char>(file),
              std::istreambuf_iterator<char>(),
              std::back_inserter(data.content));

    texts[name] = data;
    std::cout << "Текст '" << name << "' успешно загружен (" << data.content.size() << " символов)\n";
  }

  void analyze_letters(const std::string& name)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    last_analysis_start = std::chrono::steady_clock::now();

    auto& freq = texts[name].letter_freq;
    freq.clear();

    std::for_each(
      texts[name].content.begin(),
      texts[name].content.end(),
      [&freq](char c)
      {
        if (std::isalpha(c)) freq[std::tolower(c)]++;
      });

    last_analysis_end = std::chrono::steady_clock::now();
    std::cout << "Анализ букв для текста '" << name << "' завершен\n";
  }

  void analyze_words(const std::string& name)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    last_analysis_start = std::chrono::steady_clock::now();

    auto& freq = texts[name].word_freq;
    freq.clear();

    const std::string& content = texts[name].content;
    std::string current_word;

    std::for_each(
      content.begin(),
      content.end(),
      [&freq, &current_word](char c)
      {
        if (std::isalpha(c) || c == '\'')
        {
          current_word += std::tolower(c);
        }
        else
        {
          if (!current_word.empty())
          {
            freq[current_word]++;
            current_word.clear();
          }
        }
      });

    if (!current_word.empty())
    {
      freq[current_word]++;
    }

    last_analysis_end = std::chrono::steady_clock::now();
    std::cout << "Анализ слов для текста '" << name << "' завершен\n";
  }

  template <typename T>
  void print_top(const std::map<T, int>& freq, int n, const std::string& label)
  {
    std::vector<std::pair<T, int>> items(freq.begin(), freq.end());

    std::partial_sort(
      items.begin(),
      items.begin() + std::min(n,(int)items.size()),
      items.end(),
      [](const auto& a, const auto& b)
      {
        return a.second > b.second;
      });

    std::cout << "Топ-" << n << " " << label << ":\n";
    std::cout << "-------------------\n";
    std::for_each(
      items.begin(),
      items.begin() + std::min(n, (int)items.size()),
      [](const auto& item)
      {
        std::cout << std::setw(15) << std::left << item.first << ": " << item.second << '\n';
      });
    std::cout << "-------------------\n";
  }

  void analyze_letters_top(const std::string& name, int n)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    if (texts[name].letter_freq.empty()) analyze_letters(name);
    print_top(texts[name].letter_freq, n, "букв");
  }

  void analyze_words_top(const std::string& name, int n)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    if (texts[name].word_freq.empty())
    {
      analyze_words(name);
    }
    print_top(texts[name].word_freq, n, "слов");
  }

  void measure_time()
  {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(last_analysis_end - last_analysis_start);
    std::cout << "Последний анализ занял " << duration.count() << " мс\n";
  }

  void save_results(const std::string& filename)
  {
    std::ofstream file(filename);
    if (!file)
    {
      std::cerr << "Ошибка: не удалось создать файл " << filename << '\n';
      return;
    }

    std::for_each(
      texts.begin(),
      texts.end(),
      [&file](const auto& text_pair)
      {
        file << "Текст: " << text_pair.first << "\n\nЧастоты букв:\n";
        std::transform(
          text_pair.second.letter_freq.begin(),
          text_pair.second.letter_freq.end(),
          std::ostream_iterator<std::string>(file, "\n"),
          [](const auto& pair)
          {
            return std::string(1, pair.first) + ": " + std::to_string(pair.second);
          });

        file << "\nЧастоты слов:\n";
        std::transform(
          text_pair.second.word_freq.begin(),
          text_pair.second.word_freq.end(),
          std::ostream_iterator<std::string>(file, "\n"),
          [](const auto& pair)
          {
            return pair.first + ": " + std::to_string(pair.second);
          });
        file << "\n=================================\n\n";
      });

    std::cout << "Результаты сохранены в " << filename << '\n';
  }

  void list_texts()
  {
    if (texts.empty())
    {
      std::cout << "Нет загруженных текстов\n";
      return;
    }

    std::cout << "Загруженные тексты:\n-------------------\n";
    std::transform(
      texts.begin(),
      texts.end(),
      std::ostream_iterator<std::string>(std::cout, "\n"),
      [](const auto& pair)
      {
        return "- " + pair.first + " (" + std::to_string(pair.second.content.size()) + " символов, " +
          std::to_string(pair.second.word_freq.size()) + " уникальных слов)";
      });
    std::cout << "-------------------\n";
  }

  void count_symbols(const std::string& name)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    std::cout << "Текст '" << name << "' содержит " << texts[name].content.size() << " символов\n";
  }

  void count_words(const std::string& name)
  {
    if (texts.find(name) == texts.end())
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
      return;
    }

    if (texts[name].word_freq.empty())
    {
      analyze_words(name);
    }

    int total_words = std::accumulate(
      texts[name].word_freq.begin(),
      texts[name].word_freq.end(),
      0,
      [](int sum, const auto& pair)
      {
        return sum + pair.second;
      });

    std::cout << "Текст '" << name << "' содержит " << total_words << " слов";
    std::cout << "("<< texts[name].word_freq.size() << " уникальных)\n";
  }

  void clear_data()
  {
    texts.clear();
    std::cout << "Все данные очищены\n";
  }

  void delete_text(const std::string& name)
  {
    if (texts.erase(name))
    {
      std::cout << "Текст '" << name << "' удален\n";
    }
    else
    {
      std::cerr << "Ошибка: текст '" << name << "' не найден\n";
    }
  }

  void show_history(std::vector<std::string> command_history)
  {
    if (command_history.empty())
    {
      std::cout << "История команд пуста\n";
      return;
    }

    std::cout << "История команд:\n-------------------\n";
    std::copy(command_history.begin(), command_history.end(),std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "-------------------\n";
  }

  void clear_history(std::vector<std::string> command_history)
  {
    command_history.clear();
    std::cout << "История команд очищена\n";
  }

  double compare_freq(const std::map<char, int>& freq1, const std::map<char, int>& freq2)
  {
    int total1 = std::accumulate(
      freq1.begin(),
      freq1.end(),
      0,
      [](int sum, const auto& p)
      {
        return sum + p.second;
      });

    int total2 = std::accumulate(
      freq2.begin(),
      freq2.end(),
      0,
      [](int sum, const auto& p)
      {
        return sum + p.second;
      });

    double diff = std::inner_product(
      freq1.begin(),
      freq1.end(),
      freq2.begin(),
      0.0,
      std::plus<>(),
      [total1, total2](const auto& p1, const auto& p2)
      {
        return std::abs(static_cast<double>(p1.second)/total1 - static_cast<double>(p2.second)/total2);
      });

    return (1.0 - diff / 2.0) * 100.0;
  }

  void compare_letters(const std::string& name1, const std::string& name2)
  {
    if (texts.find(name1) == texts.end() || texts.find(name2) == texts.end())
    {
      std::cerr << "Ошибка: один или оба текста не найдены\n";
      return;
    }

    if (texts[name1].letter_freq.empty())
    {
      analyze_letters(name1);
    }
    if (texts[name2].letter_freq.empty())
    {
      analyze_letters(name2);
    }

    double similarity = compare_freq(texts[name1].letter_freq, texts[name2].letter_freq);
    std::cout << "Сходство частот букв: " << std::fixed << std::setprecision(2) << similarity << "%\n";
  }

  void set_default_top(int n)
  {
    if (n <= 0)
    {
      std::cerr << "Ошибка: значение должно быть положительным\n";
      return;
    }
    default_top = n;
    std::cout << "Значение топ-N по умолчанию установлено: " << n << '\n';
  }

  void show_help()
  {
    const std::vector<std::string> help_lines = {
      "Доступные команды:",
      "------------------------------------------------------------------------",
      "loadtext <filename>             - Загрузить текст из файла",
      "analyze_letters <name>          - Анализ частот букв",
      "analyze_words <name>            - Анализ частот слов",
      "analyze_letters_top <name> <N>  - Показать топ-N букв",
      "analyze_words_top <name> <N>    - Показать топ-N слов",
      "measure_time                    - Измерить время последнего анализа",
      "save_results <output>           - Сохранить результаты анализа",
      "list_texts                      - Список загруженных текстов",
      "count_symbols <name>            - Подсчет символов в тексте",
      "count_words <name>              - Подсчет слов в тексте",
      "clear_data                      - Очистить все данные",
      "delete_text <name>              - Удалить текст",
      "show_history                    - Показать историю команд",
      "clear_history                   - Очистить историю команд",
      "compare_letters <name1> <name2> - Сравнить тексты по частотам букв",
      "set_default_top <N>             - Установить значение топ-N по умолчанию",
      "show_help                       - Показать эту справку",
      "exit                            - Выйти из программы",
      "------------------------------------------------------------------------"
    };

    std::copy(
      help_lines.begin(),
      help_lines.end(),
      std::ostream_iterator<std::string>(std::cout, "\n")
      );
  }
}

bool asafov::process_command(const std::string& command, std::vector<std::string> command_history)
{
  size_t space1 = command.find(' ');
  std::string cmd = (space1 == std::string::npos) ? command : command.substr(0, space1);

  if (cmd == "loadtext")
  {
    size_t space2 = command.find(' ', space1 + 1);
    if (space1 != std::string::npos && space2 != std::string::npos)
    {
      std::string name = command.substr(space1 + 1, space2 - space1 - 1);
      std::string filename = command.substr(space2 + 1);
      load_text(name, filename);
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: loadtext <name> <filename>\n";
    }
  }
  else if (cmd == "analyze_letters")
  {
    if (space1 != std::string::npos)
    {
      analyze_letters(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: analyze_letters <name>\n";
    }
  }
  else if (cmd == "analyze_words")
  {
    if (space1 != std::string::npos)
    {
      analyze_words(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: analyze_words <name>\n";
    }
  }
  else if (cmd == "analyze_letters_top")
  {
    size_t space2 = command.find(' ', space1 + 1);
    if (space1 != std::string::npos && space2 != std::string::npos)
    {
      std::string name = command.substr(space1 + 1, space2 - space1 - 1);
      int n = std::stoi(command.substr(space2 + 1));
      analyze_letters_top(name, n);
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: analyze_letters_top <name> <N>\n";
    }
  }
  else if (cmd == "analyze_words_top")
  {
    size_t space2 = command.find(' ', space1 + 1);
    if (space1 != std::string::npos && space2 != std::string::npos)
    {
      std::string name = command.substr(space1 + 1, space2 - space1 - 1);
      int n = std::stoi(command.substr(space2 + 1));
      analyze_words_top(name, n);
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: analyze_words_top <name> <N>\n";
    }
  }
  else if (cmd == "measure_time")
  {
    measure_time();
  }
  else if (cmd == "save_results")
  {
    if (space1 != std::string::npos)
    {
      save_results(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: save_results <filename>\n";
    }
  }
  else if (cmd == "list_texts")
  {
    list_texts();
  }
  else if (cmd == "count_symbols")
  {
    if (space1 != std::string::npos)
    {
      count_symbols(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: count_symbols <name>\n";
    }
  }
  else if (cmd == "count_words")
  {
    if (space1 != std::string::npos)
    {
      count_words(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: count_words <name>\n";
    }
  }
  else if (cmd == "clear_data")
  {
    clear_data();
  }
  else if (cmd == "delete_text")
  {
    if (space1 != std::string::npos)
    {
      delete_text(command.substr(space1 + 1));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: delete_text <name>\n";
    }
  }
  else if (cmd == "show_history")
  {
    show_history(command_history);
  }
  else if (cmd == "clear_history")
  {
    clear_history(command_history);
  }
  else if (cmd == "compare_letters")
  {
    size_t space2 = command.find(' ', space1 + 1);
    if (space1 != std::string::npos && space2 != std::string::npos)
    {
      std::string name1 = command.substr(space1 + 1, space2 - space1 - 1);
      std::string name2 = command.substr(space2 + 1);
      compare_letters(name1, name2);
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: compare_letters <name1> <name2>\n";
    }
  }
  else if (cmd == "set_default_top")
  {
    if (space1 != std::string::npos)
    {
      set_default_top(std::stoi(command.substr(space1 + 1)));
    }
    else
    {
      std::cerr << "Ошибка: неверные аргументы. Используйте: set_default_top <N>\n";
    }
  }
  else if (cmd == "show_help")
  {
    show_help();
  }
  else if (cmd == "exit" || cmd == "quit")
  {
    return false;
  }
  else
  {
    std::cerr << "Ошибка: неизвестная команда. Введите 'show_help' для списка команд\n";
  }
  return true;
}
