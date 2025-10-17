#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <functional>

namespace sharifullina
{
  using Dictionary = std::unordered_map< std::string, std::set< std::string > >;
  using DictCollection = std::unordered_map< std::string, Dictionary >;

  void createDict(std::istream & in, DictCollection & dicts);
  void deleteDict(std::istream & in, DictCollection & dicts);
  void listDicts(std::istream &, const DictCollection & dicts, std::ostream & out);
  void addWord(std::istream & in, DictCollection & dicts);
  void addTranslation(std::istream & in, DictCollection & dicts);
  void removeTranslation(std::istream & in, DictCollection & dicts);
  void deleteWord(std::istream & in, DictCollection & dicts);
  void findTranslations(std::istream & in, const DictCollection & dicts, std::ostream & out);
  void listWords(std::istream & in, const DictCollection & dicts, std::ostream & out);
  void mergeDicts(std::istream & in, DictCollection & dicts);
  void findCommon(std::istream & in, const DictCollection & dicts, std::ostream & out);
  void saveDict(std::istream & in, const DictCollection & dicts, std::ostream & out);
  void loadDict(std::istream & in, DictCollection & dicts);
  void statDict(std::istream & in, const DictCollection & dicts, std::ostream & out);
  void subtractDicts(std::istream & in, DictCollection & dicts);
  void symdiffDicts(std::istream & in, DictCollection & dicts);

  void loadFile(const std::string & filename, DictCollection & dicts);
  void printHelp(std::ostream & out);
  void printError(const std::string & message, std::ostream & out);
}

#endif
