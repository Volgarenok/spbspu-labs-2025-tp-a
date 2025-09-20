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
  void addWord(const std::string& word, const std::string& freq_str, DictionaryManager& dm, std::ostream& os);
  void removeWord(const std::string& word, DictionaryManager& dm, std::ostream& os);
  void freq(const std::string& word, DictionaryManager& dm, std::ostream& os);
  void createDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os);
  void deleteDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os);
  void pushDictCmd(const std::string& name, DictionaryManager& dm, std::ostream& os);
  void popDictCmd(DictionaryManager& dm, std::ostream& os);
  void stackListCmd(DictionaryManager& dm, std::ostream& os);
  void list(DictionaryManager& dm, std::ostream& os);
  void mostFreq(const std::string& n_str, DictionaryManager& dm, std::ostream& os);
  void leastFreq(const std::string& n_str, DictionaryManager& dm, std::ostream& os);
  void mergeDictionariesCmd(const std::string& otherName, const std::string& newDictName, DictionaryManager& dm, std::ostream& os);
  void intersectDictionariesCmd(const std::string& otherName, const std::string& newDictName, DictionaryManager& dm, std::ostream& os);
  void loadFromFileCmd(const std::string& filename, DictionaryManager& dm, std::ostream& os);
  void saveToFileCmd(const std::string& filename, DictionaryManager& dm, std::ostream& os);
  void filterByPatternCmd(const std::string& regex_str, const std::string& resultDictName, DictionaryManager& dm, std::ostream& os);

  using CommandFunction = std::function< void(const std::vector< std::string >&, DictionaryManager&, std::ostream&) >;

  std::map< std::string, CommandFunction > createCommandMap(DictionaryManager& dm, std::ostream& os);
}

#endif
