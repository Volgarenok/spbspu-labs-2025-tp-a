#ifndef COMMAND_HANDLERS_HPP
#define COMMAND_HANDLERS_HPP
#include <functional>
#include <iostream>
#include "command-helpers.hpp"
#include "dictionary.hpp"

namespace belyaev
{
  void insertEntryCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void removeEntryCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void searchEntryCmd(const Dictionaries& data, std::istream& in, std::ostream& out, std::string ruOrEng);
  void searchEntryByEnglishCmd(const Dictionaries& data, std::istream& in, std::ostream& out, std::string ruOrEng);
  void searchContainsCmd(const Dictionaries& data, std::istream& in, std::ostream& out, std::string ruOrEng);
  void printDictCmd(const Dictionaries& data, std::istream& in, std::ostream& out);
  void printAllDictCmd(const Dictionaries& data, std::ostream& out);
  void clearDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void clearAllDictionariesCmd(Dictionaries& data);
  void copyDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void renameDictionaryCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void mergeDictionariesCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void intersectDictionariesCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void deleteIntersectionsCmd(Dictionaries& data, std::istream& in, std::ostream& out);
  void helpCmd(std::ostream& out);

  using commandMap = std::map< std::string, std::function< void(std::istream&, std::ostream&) > >;
  commandMap mapCommandHandlers(Dictionaries& data);
}

#endif
