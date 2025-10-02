#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <map>
#include <list>

namespace demehin
{
  using list_t = std::list< std::string >;
  using tree_t = std::map< std::string, list_t >;
  using dict_t = std::map< std::string, tree_t >;

  void printHelp(std::ostream&);
  void createDict(std::istream&, dict_t&);
  void deleteDict(std::istream&, dict_t&);
  void printDict(std::istream&, std::ostream&, const dict_t&);
  void getTranslationEng(std::istream&, std::ostream&, const dict_t&);
  void getTranslationRu(std::istream&, std::ostream&, const dict_t&);
  void deleteEng(std::istream&, dict_t&);
  void addEng(std::istream&, dict_t&);
  void deleteRu(std::istream&, dict_t&);
  void addRu(std::istream&, dict_t&);
  void makeUnion(std::istream&, dict_t&);
  void makeIntersect(std::istream&, dict_t&);
  void makeComplement(std::istream&, dict_t&);
  void writeToFile(std::istream&, const dict_t&);
  void rewriteFile(std::istream&, const dict_t&);
  void printMostCommons(std::istream&, std::ostream&, const dict_t&);
}

#endif
