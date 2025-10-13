#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <unordered_map>
#include <string>

namespace cherkasov
{
  using Dict = std::unordered_map< std::string, int >;
  using DictTable = std::unordered_map< std::string, Dict >;
  using WordEntry = std::pair< std::string, int >;

  void makeDict(std::istream & in, DictTable & dicts);
  void listDicts(std::ostream & out, const DictTable & dicts);
  void importText(std::istream & in, DictTable & dicts);
  void mergeDicts(std::istream & in, DictTable & dicts);
  void intersectDicts(std::istream & in, DictTable & dicts);
  void cloneDict(std::istream & in, DictTable & dicts);
  void insertWord(std::istream & in, DictTable & dicts);
  void dictSize(std::istream & in, std::ostream & out, const DictTable & dicts);
  void eraseWord(std::istream & in, DictTable & dicts);
  void clearDict(std::istream & in, DictTable & dicts);
  void printTop(std::istream & in, std::ostream & out, const DictTable & dicts);
  void printRare(std::istream & in, std::ostream & out, const DictTable & dicts);
  void printHelp(std::ostream& out);
  void loadFile(const std::string& filename, DictTable& dicts);
}

#endif
