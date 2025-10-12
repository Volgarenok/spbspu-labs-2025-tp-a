#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "dictionary.hpp"
#include <iostream>

namespace shak
{
  using Dictionaries = std::map< std::string, FrequencyDictionary >;
  void createDict(Dictionaries &dicts, std::istream &in);
  void clearDict(Dictionaries &dicts, std::istream &in);
  void deleteDict(Dictionaries &dicts, std::istream &in);
  void printDict(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void insertW(Dictionaries &dicts, std::istream &in);
  void removeW(Dictionaries &dicts, std::istream &in);
  void getFreq(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void getUniqe(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void mostPopular(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void equal(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void compare(Dictionaries &dicts, std::istream &in, std::ostream &out);
  void intersection(Dictionaries &dicts, std::istream &in);
}
#endif
