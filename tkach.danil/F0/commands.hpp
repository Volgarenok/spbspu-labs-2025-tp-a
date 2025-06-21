#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <map>
#include <list>
#include <string>

namespace tkach
{
  using tree_of_words = std::map< std::string, std::list< std::string > >;
  using tree_of_dict = std::map< std::string, tree_of_words >;
  void import(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void doExportOverwrite(std::istream& in, const std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void doExportInEnd(std::istream& in, const std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void addWord(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void printCommonTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void removeWord(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void clear(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void printCount(std::istream& in, std::ostream& out, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void mergeWords(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void mergeNumberDicts(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void doCommonPartDicts(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void copyTranslations(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void printTranslations(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void printEngWordsWithTraslation(std::istream& in, std::ostream& out, const tree_of_dict& avltree);
  void addTranslation(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void removeTranslation(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void substructDicts(std::istream& in, std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void printAll(std::ostream& out, const std::map< std::string, std::map< std::string, std::list< std::string > > >& avltree);
  void help(std::ostream& out);
}

#endif
