#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dictionary_types.hpp"
#include <istream>
#include <ostream>

namespace cherepkov
{
  void createDictionary(DictionarySet &dicts, std::istream &in);
  void deleteDictionary(DictionarySet &dicts, std::istream &in);
  void insert(DictionarySet &dicts, std::istream &in);
  void removeWord(DictionarySet &dicts, std::istream &in);
  void clearDictionary(DictionarySet &dicts, std::istream &in);
  void contains(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void mergeDicts(DictionarySet &dicts, std::istream &in);
  void print(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void count(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void search(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void top(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void listTop(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void uniqueMultiple(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void intersectDicts(DictionarySet &dicts, std::istream &in);
  void diffDicts(DictionarySet &dicts, std::istream &in);
  void filterByFrequency(DictionarySet &dicts, std::istream &in);
  void median(DictionarySet &dicts, std::istream &in, std::ostream &out);
}

#endif
