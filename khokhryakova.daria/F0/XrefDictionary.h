#ifndef XREF_DICTIONARY_HPP
#define XREF_DICTIONARY_HPP

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace khokhryakova
{
  struct Position
  {
    size_t line;
    size_t col;
  };

  struct OneXrefDict
  {
    std::map< std::string, std::vector< Position > > dictionary;
    std::vector< std::vector<std::string > > text;
  };

  struct XrefDictionary
  {
    std::unordered_map< std::string, OneXrefDict > dicts;
  };

  void buildXref(std::istream& in, std::ostream& out, XrefDictionary& dict);
  void printDict(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void findWord(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void countWord(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void getContext(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void mergeXref(std::istream& in, std::ostream& out, XrefDictionary& dict);
  void intersectXref(std::istream& in, std::ostream& out, XrefDictionary& dict);
  void diffXref(std::istream& in, std::ostream& out, XrefDictionary& dict);
  void saveXref(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void loadXref(std::istream& in, std::ostream& out, XrefDictionary& dict);
  void loadXrefFromPath(std::ostream& out, const std::string& fileName, XrefDictionary& dict);
  void restoreText(std::istream& in, std::ostream& out, const XrefDictionary& dict);
  void showHelp(std::istream& in, std::ostream& out);
}

#endif
