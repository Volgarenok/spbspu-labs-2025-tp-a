#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <fstream>
#include <string>
#include <unordered_map>

namespace filonova
{
  using Dictionary = std::unordered_map< std::string, size_t >;
  using DictionarySet = std::unordered_map< std::string, Dictionary >;

  void createDictionary(DictionarySet &dicts, std::istream &in);
  void deleteDictionary(DictionarySet &dicts, std::istream &in);
  void insert(DictionarySet &dicts, std::istream &in);
  void remove(DictionarySet &dicts, std::istream &in);
  void clear(DictionarySet &dicts, std::istream &in);
  void contains(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void merge(DictionarySet &dicts, std::istream &in);
  void print(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void count(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void top(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void unique(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void mostrare(DictionarySet &dicts, std::istream &in, std::ostream &out);
  void intersectDictionary(DictionarySet &dicts, std::istream &in);
  void excludeDictionary(DictionarySet &dicts, std::istream &in);
}

#endif
