#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <string>
#include <unordered_map>

namespace shchadilov
{
  using FrequencyDictionary = std::unordered_map< std::string, std::size_t >;
  using DictionarySet = std::unordered_map< std::string, FrequencyDictionary >;

  void help(std::ostream& out);
  void create(DictionarySet& set, std::istream& in);
  void dismiss(DictionarySet& set, std::istream& in);
  void list_dicts(const DictionarySet& set, std::ostream& out);
  void build(DictionarySet& set, std::istream& in);
  void show(const DictionarySet& set, std::istream& in, std::ostream& out);
  void clear(DictionarySet& set, std::istream& in);
  void merge(DictionarySet& set, std::istream& in, std::ostream& out);
  void intersect(DictionarySet& set, std::istream& in);
  void save(const DictionarySet& set, std::istream& in);
  void load(DictionarySet& set, std::istream& in);
  void wordfreq(const DictionarySet& set, std::istream& in, std::ostream& out);
  void topwords(const DictionarySet& set, std::istream& in, std::ostream& out);
  void diff(DictionarySet& set, std::istream& in, std::ostream& out);
  void unionavg(DictionarySet& set, std::istream& in, std::ostream& out);
  void compare(const DictionarySet& set, std::istream& in, std::ostream& out);
  void medianword(const DictionarySet& set, std::istream& in, std::ostream& out);
}

#endif
