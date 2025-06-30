#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <map>
#include <list>
#include <fstream>
#include <iostream>

namespace orlova
{
  using Translation = std::list< std::string >;
  using Dictionary = std::map< std::string, Translation >;
  using Dictionaries = std::map< std::string, Dictionary >;
  void addEmptyDictionary(std::istream&, std::ostream&, Dictionaries&);
  void addTranslation(std::istream&, std::ostream&, Dictionaries&);
  void addDictionary(std::istream&, std::ostream&, Dictionaries&);
  void find(std::istream&, std::ostream&, const Dictionaries&);
  template < typename Pair >
  void merge(std::istream&, std::ostream&, Dictionaries&);
  void clear(std::istream&, std::ostream&, Dictionaries&);
  void removeWord(std::istream&, std::ostream&, Dictionaries&);
  void append(std::istream&, std::ostream&, Dictionaries&);
  void dictionarySize(std::istream&, std::ostream&, const Dictionaries&);
  template < typename Pair >
  void intersectionOfDicts(std::istream&, std::ostream&, Dictionaries&);
  void delete_(std::istream&, std::ostream&, Dictionaries&);
  template < typename Pair >
  void nonrepeatingWords(std::istream&, std::ostream&, Dictionaries&);
  template < typename Pair >
  void residual(std::istream&, std::ostream&, Dictionaries&);
}

#endif
