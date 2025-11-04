#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

namespace nehvedovich
{
  using Dictionary = std::unordered_map< std::string, std::vector< std::string > >;
  using DictionarySet = std::unordered_map< std::string, Dictionary >;

  void createDict(std::istream& in, DictionarySet& dicts);
  void deleteDict(std::istream& in, DictionarySet& dicts);
  void add(std::istream& in, DictionarySet& dicts);
  void remove(std::istream& in, DictionarySet& dicts);
  void translate(std::istream& in, const DictionarySet& dicts, std::ostream& out);
  void contains(std::istream& in, const DictionarySet& dicts, std::ostream& out);
  void list(std::istream& in, const DictionarySet& dicts, std::ostream& out);
  void merge(std::istream& in, DictionarySet& dicts);
  void quantity(std::istream& in, const DictionarySet& dicts, std::ostream& out);
  void reverseTranslate(std::istream& in, const DictionarySet& dicts, std::ostream& out);
  void edit(std::istream& in, DictionarySet& dicts);
  void moreTranslations(std::istream& in, const DictionarySet& dicts, std::ostream& out);
}

#endif
