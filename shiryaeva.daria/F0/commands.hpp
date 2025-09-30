#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dictionary.hpp"
#include <map>
#include <string>
#include <istream>
#include <ostream>

namespace shiryaeva
{
  using Dictionaries = std::map< std::string, FrequencyDictionary >;

  void process_text_and_add(FrequencyDictionary &dict, const std::string &text);
  void create_dict(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void add_text(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void add_string(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void get_freq(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void get_rel_freq(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void remove_word(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void filter_by_freq(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void exclude_by_freq(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void display_all(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void clear_dict(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void merge_dicts(Dictionaries &dicts, std::istream &args, std::ostream &out);
  void most_common(Dictionaries &dicts, std::istream &args, std::ostream &out);
}

#endif
