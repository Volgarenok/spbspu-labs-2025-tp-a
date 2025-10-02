#ifndef SHANNONFANO_HPP
#define SHANNONFANO_HPP

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <iostream>

namespace duhanina
{
  struct CodeTable
  {
    std::map< char, std::string > char_to_code;
    std::map< std::string, char > code_to_char;
    size_t total_chars = 0;
  };

  struct Node
  {
    char symbol;
    size_t freq;
    Node* left;
    Node* right;

    Node(char s, size_t f):
      symbol(s),
      freq(f),
      left(nullptr),
      right(nullptr)
    {}

    Node(size_t f, Node* l, Node* r):
      symbol(0),
      freq(f),
      left(l),
      right(r)
    {}
  };

  using str_t = const std::string&;

  void print_help(std::ostream& out);
  void build_codes(str_t input_file, str_t encoding_name, std::ostream& out);
  void show_codes(str_t encoding_name, std::ostream& out);
  void save_codes(str_t encoding_name, str_t output_file, std::ostream& out);
  void load_codes(str_t input_file, str_t encoding_name, std::ostream& out);
  void clear_codes(str_t encoding_name, std::ostream& out);
  void encode_file_with_codes(str_t input_file, str_t encoding_name, str_t output_file, std::ostream& out);
  void decode_file_with_codes(str_t input_file, str_t encoding_name, str_t output_file, std::ostream& out);
  void encode_file(str_t input_file, str_t output_file, str_t encoding_name, std::ostream& out);
  void decode_file(str_t input_file, str_t output_file, str_t encoding_name, std::ostream& out);
  void compare(str_t file1, str_t file2, str_t encod_name1, str_t encod_name2, std::ostream& out);
  void list_encodings(std::ostream& out);
  void check_encoding(str_t input_file, str_t encoding_id, std::ostream& out);
  void suggest_encodings(str_t input_file, std::ostream& out);
}

#endif
