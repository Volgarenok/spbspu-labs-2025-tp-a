#ifndef COMMANDS_H
#define COMMANDS_H
#include <iostream>
#include <functional>
#include <list>
#include <map>

namespace fedorova
{
  using namespace std::placeholders;
  using WordSet = std::list< std::string >;
  using Dictionary = std::map< std::string, WordSet >;
  using DictSet = std::map< std::string, Dictionary >;
  using DictContent = std::map< char, std::list< std::string > >;

  void add_word(std::istream& is, std::ostream& os, DictSet& set);
  void delete_word(std::istream& is, std::ostream& os, DictSet& set);
  void translate(std::istream& is, std::ostream& os, const DictSet& set);
  void see_dict(std::istream& is, std::ostream& os, const DictSet& set);
  void start_with(std::istream& is, std::ostream& os, const DictSet& set);
  void change_translation(std::istream& is, std::ostream& os, DictSet& set);
  void delete_translate(std::istream& is, std::ostream& os, DictSet& set);
  void merge(std::istream& is, std::ostream& os, DictSet& set);
  void export_range(std::istream& is, std::ostream& os, const DictSet& set);
  void translate_in_dicts(std::istream& is, std::ostream& os, const DictSet& set);
  void create_dict(std::istream& is, std::ostream& os, DictSet& set);
  void delete_dict(std::istream& is, std::ostream& os, DictSet& set);
  void list_dicts(std::ostream& os, const DictSet& set);
  void size_of_dict(std::istream& is, std::ostream& os, const DictSet& set);
  void contains(std::istream& is, std::ostream& os, const DictSet& set);
  void add_translate(std::istream& is, std::ostream& os, DictSet& set);
  void export_no_range(std::istream& is, std::ostream& os, const DictSet& set);

  void print_help(std::ostream& os);
}

#endif
