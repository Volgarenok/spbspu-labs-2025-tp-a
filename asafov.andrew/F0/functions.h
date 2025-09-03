#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <map>
#include <string>
#include <vector>

namespace asafov
{
  struct TextData
  {
    std::string content;
    std::map<char, int> letter_freq;
    std::map<std::string, int> word_freq;
  };

  bool process_command(const std::string& command, std::vector<std::string>);
}
#endif
