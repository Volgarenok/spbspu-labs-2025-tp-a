#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "utilities.hpp"

namespace nikitin
{
  void createDict(std::istream &in, std::ostream &out, dictionaries &dicts);
  void deleteDict(std::istream &in, std::ostream &out, dictionaries &dicts);
  void loadCommand(std::istream &in, std::ostream &out, dictionaries &dicts);
  void saveDict(std::istream &in, std::ostream &out, const dictionaries &dicts);

  void insertWord(std::istream &in, std::ostream &out, dictionaries &dicts);
  void removeWord(std::istream &in, std::ostream &out, dictionaries &dicts);
  void insertTranslation(std::istream &in, std::ostream &out, dictionaries &dicts);
  void removeTranslation(std::istream &in, std::ostream &out, dictionaries &dicts);

  void search(std::istream &in, std::ostream &out, const dictionaries &dicts);
  void printSize(std::istream &in, std::ostream &out, const dictionaries &dicts);
  void printDict(std::istream &in, std::ostream &out, const dictionaries &dicts);

  void subtract(std::istream &in, std::ostream &out, dictionaries &dicts);
  void intersect(std::istream &in, std::ostream &out, dictionaries &dicts);
  void unite(std::istream &in, std::ostream &out, dictionaries &dicts);
}

#endif
