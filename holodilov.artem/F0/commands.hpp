#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include "Dictionary.hpp"

namespace holodilov
{
  void createDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void exportDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void importDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void clearDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void deleteDict(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void addWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void deleteWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void updateWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void printDict(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries);

  void findWord(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void merge(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void intersect(std::istream& in, std::ostream& out, std::map< std::string, Dictionary >& dictionaries);

  void exportAlphabet(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries);

  void checkAlphabet(std::istream& in, std::ostream& out, const std::map< std::string, Dictionary >& dictionaries);

  void printDictNames(std::ostream& out, const std::map< std::string, Dictionary >& dictionaries);
}

#endif
