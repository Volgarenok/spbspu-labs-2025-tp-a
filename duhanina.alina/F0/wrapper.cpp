#include "wrapper.hpp"
#include "shannonFano.hpp"

void duhanina::build_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string encoding_name;
  if (!(in >> input_file >> encoding_name))
  {
    throw std::runtime_error("Invalid arguments");
  }
  build_codes(input_file, encoding_name, out);
}

void duhanina::show_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string encoding_name;
  if (!(in >> encoding_name))
  {
    throw std::runtime_error("Invalid arguments");
  }
  show_codes(encoding_name, out);
}

void duhanina::save_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string encoding_name;
  std::string output_file;
  if (!(in >> encoding_name >> output_file))
  {
    throw std::runtime_error("Invalid arguments");
  }
  save_codes(encoding_name, output_file, out);
}

void duhanina::load_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string encoding_name;
  if (!(in >> input_file >> encoding_name))
  {
    throw std::runtime_error("Invalid arguments");
  }
  load_codes(input_file, encoding_name, out);
}

void duhanina::clear_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string encoding_name;
  if (!(in >> encoding_name))
  {
    throw std::runtime_error("Invalid arguments");
  }
  clear_codes(encoding_name, out);
}

void duhanina::encode_file_with_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string encoding_name;
  std::string output_file;
  if (!(in >> input_file >> encoding_name >> output_file))
  {
    throw std::runtime_error("Invalid arguments");
  }
  encode_file_with_codes(input_file, encoding_name, output_file, out);
}

void duhanina::decode_file_with_codes_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string encoding_name;
  std::string output_file;
  if (!(in >> input_file >> encoding_name >> output_file))
  {
    throw std::runtime_error("Invalid arguments");
  }
  decode_file_with_codes(input_file, encoding_name, output_file, out);
}

void duhanina::encode_file_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string output_file;
  std::string encoding_name;
  if (!(in >> input_file >> output_file >> encoding_name))
  {
    throw std::runtime_error("Invalid arguments");
  }
  encode_file(input_file, output_file, encoding_name, out);
}

void duhanina::decode_file_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string output_file;
  std::string encoding_name;
  if (!(in >> input_file >> output_file >> encoding_name))
  {
  throw std::runtime_error("Invalid arguments");
  }
  decode_file(input_file, output_file, encoding_name, out);
}

void duhanina::compare_wrapper(std::istream& in, std::ostream& out)
{
  std::string file1;
  std::string file2;
  std::string encoding_name1;
  std::string encoding_name2;
  if (!(in >> file1 >> file2 >> encoding_name1 >> encoding_name2))
  {
    throw std::runtime_error("Invalid arguments");
  }
  compare(file1, file2, encoding_name1, encoding_name2, out);
}

void duhanina::suggest_encodings_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  if (!(in >> input_file))
  {
    throw std::runtime_error("Invalid arguments");
  }
  suggest_encodings(input_file, out);
}

void duhanina::check_encoding_wrapper(std::istream& in, std::ostream& out)
{
  std::string input_file;
  std::string encoding_id;
  if (!(in >> input_file >> encoding_id))
  {
    throw std::runtime_error("Invalid arguments");
  }
  check_encoding(input_file, encoding_id, out);
}

std::map< std::string, std::function< void() > > duhanina::initialize_commands(std::istream& in, std::ostream& out)
{
  std::map< std::string, std::function< void() > > commands;

  commands["build_codes"] = std::bind(build_codes_wrapper, std::ref(in), std::ref(out));
  commands["show_codes"] = std::bind(show_codes_wrapper, std::ref(in), std::ref(out));
  commands["save_codes"] = std::bind(save_codes_wrapper, std::ref(in), std::ref(out));
  commands["load_codes"] = std::bind(load_codes_wrapper, std::ref(in), std::ref(out));
  commands["clear_codes"] = std::bind(clear_codes_wrapper, std::ref(in), std::ref(out));
  commands["encode_file_with_codes"] = std::bind(encode_file_with_codes_wrapper, std::ref(in), std::ref(out));
  commands["decode_file_with_codes"] = std::bind(decode_file_with_codes_wrapper, std::ref(in), std::ref(out));
  commands["encode_file"] = std::bind(encode_file_wrapper, std::ref(in), std::ref(out));
  commands["decode_file"] = std::bind(decode_file_wrapper, std::ref(in), std::ref(out));
  commands["compare"] = std::bind(compare_wrapper, std::ref(in), std::ref(out));
  commands["list_encodings"] = std::bind(list_encodings, std::ref(out));
  commands["suggest"] = std::bind(suggest_encodings_wrapper, std::ref(in), std::ref(out));
  commands["check"] = std::bind(check_encoding_wrapper, std::ref(in), std::ref(out));
  return commands;
}
