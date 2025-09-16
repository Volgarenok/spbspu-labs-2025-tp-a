#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dictionary.hpp"
#include <string>
#include <map>
#include <vector>

class CommandHandler
{
public:
  void handleCommand(const std::string &line);

private:
  std::map<std::string, Dictionary> dictionaries_;

  void printHelp() const;
  void printStats(const Dictionary &dict) const;
  void quiz(const Dictionary &dict, std::size_t count) const;
  void exportDict(const Dictionary &dict, const std::string &filename) const;
  void importDict(Dictionary &dict, const std::string &filename);
  void compareDicts(const Dictionary &dict1, const Dictionary &dict2) const;
};

#endif
