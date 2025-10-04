#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include "Dictionary.hpp"

namespace holodilov
{
  void createDict(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void exportDict(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void importDict(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void clearDict(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void deleteDict(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void addWord(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void deleteWord(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void updateWord(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void printDict(std::istream& in, std::ostream& out, const MapDicts& dictionaries);

  void findWord(std::istream& in, std::ostream& out, MapDicts& mapDicts);

  void merge(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void intersect(std::istream& in, std::ostream& out, MapDicts& dictionaries);

  void exportAlphabet(std::istream& in, std::ostream& out, const MapDicts& dictionaries);

  void checkAlphabet(std::istream& in, std::ostream& out, const MapDicts& dictionaries);

  void printDictNames(std::ostream& out, const MapDicts& dictionaries);
}

#endif
