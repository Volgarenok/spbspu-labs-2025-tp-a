#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <cctype>

namespace khoroshilov
{
  using list_t = std::list<std::string>;
  using tree_t = std::map<std::string, list_t>;
  using dict_t = std::map<std::string, tree_t>;

  void printHelp(std::ostream& out);
  void load(std::istream& in, dict_t& dicts);
  void save(std::istream& in, const dict_t& dicts);
  void dictcreate(std::istream& in, dict_t& dicts);
  void dictrm(std::istream& in, dict_t& dicts);
  void lsdict(std::istream& in, std::ostream& out, const dict_t& dicts);
  void engtranslate(std::istream& in, std::ostream& out, const dict_t& dicts);
  void addenglish(std::istream& in, dict_t& dicts);
  void addru(std::istream& in, dict_t& dicts);
  void rmenglish(std::istream& in, dict_t& dicts);
  void rmru(std::istream& in, dict_t& dicts);
  void maxlen(std::istream& in, std::ostream& out, const dict_t& dicts);
  void alfrange(std::istream& in, std::ostream& out, const dict_t& dicts);
  void engcount(std::istream& in, std::ostream& out, const dict_t& dicts);
  void prefix(std::istream& in, std::ostream& out, const dict_t& dicts);
  void clear(std::istream& in, std::ostream& out, dict_t& dicts);
  void complement(std::istream& in, dict_t& dicts);
  void intersect(std::istream& in, dict_t& dicts);
  void longest(std::istream& in, dict_t& dicts);
  void meancount(std::istream& in, std::ostream& out, const dict_t& dicts);
  void meaningful(std::istream& in, dict_t& dicts);
}

#endif
