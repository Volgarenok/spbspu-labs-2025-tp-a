#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <exception>
#include <iosfwd>
#include <list>
#include <map>
#include <vector>

namespace zholobov {

  using Word = std::string;
  using Words = std::list< Word >;
  using Dictionary = std::map< Word, Words >;
  using Dictionaries = std::map< std::string, Dictionary >;

  void printHelp(std::ostream& out);

  void cmdDictCreate(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictRemove(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictImport(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictExport(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictCount(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictList(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictPrintWords(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictPrintTranslations(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdDictClear(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdAddWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdRemoveWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdCountWords(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdAddTranslation(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdRemoveTranslation(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdChangeWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdTranslateWord(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdUnion(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdIntersect(std::istream& in, std::ostream& out, Dictionaries& dictionaries);
  void cmdRare(std::istream& in, std::ostream& out, Dictionaries& dictionaries);

}

#endif
