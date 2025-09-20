#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <functional>
#include <list>
#include <map>

namespace alymova
{
  using namespace std::placeholders;
  using WordSet = std::list< std::string >;
  using Dictionary = std::map< std::string, WordSet >;
  using DictSet = std::map< std::string, Dictionary >;
  using ContentDict = std::map< char, std::list< std::string > >;

  void create(std::istream& in, std::ostream& out, DictSet& set);
  void dicts(std::ostream& out, const DictSet& set);
  void size(std::istream& in, std::ostream& out, const DictSet& set);
  void removeDict(std::istream& in, std::ostream& out, DictSet& set);
  void addWord(std::istream& in, std::ostream& out, DictSet& set);
  void fixWord(std::istream& in, std::ostream& out, DictSet& set);
  void findWord(std::istream& in, std::ostream& out, const DictSet& set);
  void containSubword(std::istream& in, std::ostream& out, const DictSet& set);
  void removeWord(std::istream& in, std::ostream& out, DictSet& set);
  void addTranslate(std::istream& in, std::ostream& out, DictSet& set);
  void findEnglishEquivalent(std::istream& in, std::ostream& out, const DictSet& set);
  void removeTranslate(std::istream& in, std::ostream& out, DictSet& set);
  void printContent(std::istream& in, std::ostream& out, const DictSet& set);
  void translate(std::istream& in, std::ostream& out, const DictSet& set);
  void unionDicts(std::istream& in, std::ostream& out, DictSet& set);
  void intersectDicts(std::istream& in, std::ostream& out, DictSet& set);
  void printDayWord(std::istream& in, std::ostream& out, const DictSet& set);

  void printHelp(std::ostream& out);
}

#endif
