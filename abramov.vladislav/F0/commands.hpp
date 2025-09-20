#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <functional>
#include "dictionary_collection.hpp"

namespace abramov
{
  void getCommands(std::map< std::string, std::function< void() > > &commands, DictionaryCollection &collect);
  void addDict(DictionaryCollection &collect, std::istream &in);
  void addToDict(DictionaryCollection &collect, std::istream &in);
  void deleteDict(DictionaryCollection &collect, std::istream &in);
  void deleteFromDict(DictionaryCollection &collect, std::istream &in);
  void setDicts(DictionaryCollection &collect, std::istream &in);
  void intersectDicts(DictionaryCollection &collect, std::istream &in);
  void unionDicts(DictionaryCollection &collect, std::istream &in);
  void diffDicts(DictionaryCollection &collect, std::istream &in);
  void mergeDicts(DictionaryCollection &collect, std::istream &in);
  void printDict(const DictionaryCollection &collect, std::istream &in, std::ostream &out);

  template< class Func >
  void processDicts(DictionaryCollection &collect, std::istream &in, Func func, bool del = false);
}
#endif
