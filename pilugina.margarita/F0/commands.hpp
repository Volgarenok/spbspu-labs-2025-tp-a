#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "utilities.hpp"

namespace pilugina
{
  void createDict(std::istream &in, std::ostream &out, dictionaries &dicts);
  void deleteDict(std::istream &in, std::ostream &out, dictionaries &dicts);

  void insertWord(std::istream &in, std::ostream &out, dictionaries &dicts);
  void insertTranslation(std::istream &in, std::ostream &out, dictionaries &dicts);
  void removeWord(std::istream &in, std::ostream &out, dictionaries &dicts);
  void removeTranslation(std::istream &in, std::ostream &out, dictionaries &dicts);
  void searchTranslation(std::istream &in, std::ostream &out, const dictionaries &dicts);

  void printSizeDict(std::istream &in, std::ostream &out, const dictionaries &dicts);
  void printDict(std::istream &in, std::ostream &out, const dictionaries &dicts);
  void saveDict(std::istream &in, std::ostream &out, const dictionaries &dicts);

  void mergeDicts(std::istream &in, std::ostream &out, dictionaries &dicts);
  void joinDicts(std::istream &in, std::ostream &out, dictionaries &dicts);
  void innerMerge(std::istream &in, std::ostream &out, dictionaries &dicts);
  void mergeTranslations(std::istream &in, std::ostream &out, dictionaries &dicts);
}

#endif
