#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace asafov
{
  struct State
  {
    std::map< std::string, std::string > texts;
    std::vector< std::string > history;
    std::map< std::string, std::map< std::string, int > > lastResults;
    std::chrono::duration< double > lastDuration{};
    int defaultTop = 5;
  };

  void loadtext(State &state, const std::string &filename);

  void analyze_letters(State &state, const std::string &name);

  void analyze_words(State &state, const std::string &name);

  void analyze_letters_top(State &state, const std::string &name, int N);

  void analyze_words_top(State &state, const std::string &name, int N);

  void measure_time(State &state);

  void save_results(State &state, const std::string &output);

  void list_texts(State &state);

  void count_symbols(State &state, const std::string &name);

  void count_words(State &state, const std::string &name);

  void clear_data(State &state);

  void delete_text(State &state, const std::string &name);

  void show_history(State &state);

  void clear_history(State &state);

  void compare_letters(State &state, const std::string &n1, const std::string &n2);

  void set_default_top(State &state, int N);

  void show_help();

  std::vector< std::string > splitCommand(const std::string &line);
}

#endif
