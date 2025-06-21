#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include <memory>
#include "dictionary_manager.hpp"

namespace smirnov
{
  using CommandFunction = std::function< void(std::istream&, DictionaryManager&, std::ostream&) >;

  void addWord(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void removeWord(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void freq(std::istream& is, DictionaryManager& dm, std::ostream& os);

  void createDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void deleteDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void pushDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void popDictCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void stackListCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void list(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void mostFreq(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void leastFreq(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void mergeCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void intersectCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void loadCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void saveCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);
  void filterByPatternCmd(std::istream& is, DictionaryManager& dm, std::ostream& os);

  std::map< std::string, CommandFunction > createCommandMap();

}

#endif
