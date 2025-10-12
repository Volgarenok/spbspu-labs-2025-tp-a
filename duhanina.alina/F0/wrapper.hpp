#ifndef WRAPPER_HPP
#define WRAPPER_HPP

#include <iostream>
#include <map>
#include <functional>

namespace duhanina
{
  void build_codes_wrapper(std::istream&, std::ostream&);
  void show_codes_wrapper(std::istream&, std::ostream&);
  void save_codes_wrapper(std::istream&, std::ostream&);
  void load_codes_wrapper(std::istream&, std::ostream&);
  void clear_codes_wrapper(std::istream&, std::ostream&);
  void encode_file_with_codes_wrapper(std::istream&, std::ostream&);
  void decode_file_with_codes_wrapper(std::istream&, std::ostream&);
  void encode_file_wrapper(std::istream&, std::ostream&);
  void decode_file_wrapper(std::istream&, std::ostream&);
  void compare_wrapper(std::istream&, std::ostream&);
  void suggest_encodings_wrapper(std::istream& in, std::ostream& out);
  void check_encoding_wrapper(std::istream& in, std::ostream& out);

  std::map< std::string, std::function< void() > > initialize_commands(std::istream& in, std::ostream& out);
}

#endif
