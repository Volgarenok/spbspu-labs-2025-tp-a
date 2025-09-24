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

  struct InvalidParams: public std::exception {};

  void printHelp(std::ostream& out);

  void cmdDictCreate(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictRemove(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictImport(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictExport(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictCount(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictList(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictPrintWords(Dictionaries& dictionaries, const std::vector< std::string >& args);
  void cmdDictPrintTranslations(Dictionaries& dictionaries, const std::vector< std::string >& args);

}

#endif
