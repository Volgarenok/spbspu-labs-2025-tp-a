#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <map>
#include <list>
#include <fstream>
#include <istream>
#include <ostream>
#include <functional>

namespace orlova
{
  using namespace std::placeholders;
  using Translations = std::list< std::string >;
  using Dictionary = std::map< std::string, Translations >;
  using Dictionaries = std::map< std::string, Dictionary >;
  void addEmptyDictionary(std::istream&, std::ostream&, Dictionaries&);
  void addTranslation(std::istream&, std::ostream&, Dictionaries&);
  void addDictionary(std::istream&, std::ostream&, Dictionaries&);
  void find(std::istream&, std::ostream&, const Dictionaries&);
  void merge(std::istream&, std::ostream&, Dictionaries&);
  void clear(std::istream&, std::ostream&, Dictionaries&);
  void removeWord(std::istream&, std::ostream&, Dictionaries&);
  void append(std::istream&, std::ostream&, Dictionaries&);
  void dictionarySize(std::istream&, std::ostream&, const Dictionaries&);
  void intersectionOfDicts(std::istream&, std::ostream&, Dictionaries&);
  void deleteDictionary(std::istream&, std::ostream&, Dictionaries&);
  void nonrepeatingWords(std::istream&, std::ostream&, Dictionaries&);
  void residual(std::istream&, std::ostream&, Dictionaries&);
  void printDictionary(std::istream&, std::ostream&, const Dictionaries&);
}

#endif
