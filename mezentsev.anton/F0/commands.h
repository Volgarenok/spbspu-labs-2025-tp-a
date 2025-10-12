#ifndef COMMANDS_H
#define COMMANDS_H

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace mezentsev
{
  using Translations = std::set< std::string >;
  using Dictionary = std::map< std::string, Translations >;
  using DictionarySet = std::map< std::string, Dictionary >;

  std::vector< std::string > split(const std::string& s, char delim);
  void addCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void removeCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void translateCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void listCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void saveCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void loadCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void countCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void clearCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void suggestCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void mergeCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void diffCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void copyCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void intersectCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
  void exportCommand(DictionarySet& dicts, const std::vector< std::string >& tokens);
}
#endif
