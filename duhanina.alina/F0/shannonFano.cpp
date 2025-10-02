#include "shannonFano.hpp"
#include <fstream>
#include <iterator>
#include <set>
#include <limits>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <algorithm>
#include "functor.hpp"

namespace
{
  using str_t = const std::string&;
  using namespace duhanina;

  std::map< std::string, duhanina::CodeTable > encoding_store;

  const std::string TEXT_EXT = ".txt";
  const std::string COMPRESSED_EXT = ".sfano";
  const std::string CODE_TABLE_EXT = ".sfcodes";

  void validate_extension(str_t filename, str_t expected_ext)
  {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos || filename.substr(dot_pos) != expected_ext)
    {
      throw std::runtime_error("Invalid file extension. Expected: " + expected_ext);
    }
  }

  bool compare_nodes(const std::pair< char, size_t >& a, const std::pair< char, size_t >& b)
  {
    return a.second > b.second;
  }

  void build_shannon_fano_codes(duhanina::Node* node, std::string code, std::map< char, std::string >& codes)
  {
    if (!node)
    {
      return;
    }
    if (node->left == nullptr && node->right == nullptr)
    {
      codes[node->symbol] = code;
      return;
    }
    if (node->left != nullptr)
    {
      build_shannon_fano_codes(node->left, code + "0", codes);
    }
    if (node->right != nullptr)
    {
      build_shannon_fano_codes(node->right, code + "1", codes);
    }
  }

  duhanina::CodeTable build_code_table(str_t text)
  {
    if (text.empty())
    {
      throw std::runtime_error("EMPTY");
    }
    std::map< char, size_t > freq_map;
    FreqTransformer counter(freq_map);
    std::vector< char > temp(text.begin(), text.end());
    std::transform(temp.begin(), temp.end(), temp.begin(), counter);
    if (freq_map.size() == 1)
    {
      throw std::runtime_error("SINGLE_SYMBOL");
    }
    std::vector< std::pair< char, size_t > > sorted_freq(freq_map.begin(), freq_map.end());
    std::sort(sorted_freq.begin(), sorted_freq.end(), compare_nodes);
    std::vector< Node* > nodes;
    NodeCreator creator;
    try
    {
      std::transform(sorted_freq.begin(), sorted_freq.end(), std::back_inserter(nodes), creator);
      TreeBuilder builder(nodes);
      builder();
      duhanina::CodeTable table;
      table.total_chars = text.size();
      build_shannon_fano_codes(nodes[0], "", table.char_to_code);
      std::vector< std::pair< char, std::string > > temp_vec(table.char_to_code.begin(), table.char_to_code.end());
      TableTransformer filler(table);
      std::for_each(temp_vec.begin(), temp_vec.end(), filler);
      TreeDeleter deleter;
      deleter(nodes[0]);
      return table;
    }
    catch (...)
    {
      TreeDeleter deleter;
      std::vector< duhanina::Node* > nodes_to_delete = nodes;
      std::for_each(nodes_to_delete.begin(), nodes_to_delete.end(), deleter);
      throw;
    }
  }

  std::string encode_text(str_t text, const duhanina::CodeTable& table)
  {
    std::string encoded;
    CharEncoder encoder(table, encoded);
    try
    {
      std::for_each(text.begin(), text.end(), encoder);
    }
    catch (const std::runtime_error&)
    {
      encoded.clear();
      throw;
    }
    return encoded;
  }

  std::string decode_text(str_t encoded, const duhanina::CodeTable& table)
  {
    std::string decoded;
    std::string current_code;
    CodeAccumulator accumulator(decoded, current_code, table);
    std::for_each(encoded.begin(), encoded.end(), accumulator);
    if (!current_code.empty())
    {
      throw std::runtime_error("INVALID_CODES");
    }
    return decoded;
  }

  void write_bits_to_file(str_t bits, str_t filename)
  {
    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    write_size_t(out, bits.size());
    BitWriter bit_writer(out);
    std::for_each(bits.begin(), bits.end(), bit_writer);
    bit_writer.flush();
  }

  std::string read_bits_from_file(str_t filename)
  {
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    size_t bit_count = 0;
    HeaderReader header_reader(in, bit_count);
    std::vector< int > dummy(sizeof(size_t));
    std::for_each(dummy.begin(), dummy.end(), header_reader);
    DataProcessor processor(bit_count);
    std::istreambuf_iterator< char > begin(in);
    std::istreambuf_iterator< char > end;
    std::for_each(begin, end, std::ref(processor));
    if (!processor.is_complete())
    {
      throw std::runtime_error("TRUNCATED_FILE");
    }
    return processor.get_result();
  }

  void save_code_table(const duhanina::CodeTable& table, str_t filename)
  {
    std::ofstream out(filename);
    if (!out)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    out << table.total_chars << "\n";
    TableEntryWriter writer(out);
    std::for_each(table.char_to_code.begin(), table.char_to_code.end(), writer);
  }


  duhanina::CodeTable load_code_table(str_t filename)
  {
    std::ifstream in(filename);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    duhanina::CodeTable table;
    size_t lines_processed = 0;
    in >> table.total_chars;
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    CodeLoader loader(table, lines_processed);
    loader.process(in, lines_processed);
    return table;
  }

  void encode_file_impl(str_t input_file, str_t output_file, const duhanina::CodeTable& table, std::ostream& out)
  {
    std::ifstream in(input_file, std::ios::binary);
    if (!in)
    {
      throw std::runtime_error("FILE_NOT_FOUND");
    }
    std::string text((std::istreambuf_iterator< char >(in)), std::istreambuf_iterator< char >());
    std::string encoded = encode_text(text, table);
    write_bits_to_file(encoded, output_file);
    double original_size = text.size();
    double compressed_size = std::ceil(encoded.size() / 8.0) + sizeof(size_t);
    double ratio = (compressed_size / original_size) * 100;
    out << "File successfully compressed:\n";
    out << "Original size: " << original_size << " bytes\n";
    out << "Compressed size: " << compressed_size << " bytes\n";
    out << "Compression ratio: " << std::fixed << std::setprecision(2) << ratio << "%\n";
  }

  void decode_file_impl(str_t input_file, str_t output_file, const duhanina::CodeTable& table, std::ostream& out)
  {
    std::string encoded = read_bits_from_file(input_file);
    std::string decoded = decode_text(encoded, table);
    std::ofstream out_file(output_file);
    if (!out_file)
    {
      throw std::runtime_error("INVALID_FILE");
    }
    out_file << decoded;
    out << "File successfully decompressed to '" << output_file << "'\n";
  }

  void print_missing_chars(const std::set< char >& missing, std::ostream& out)
  {
    if (missing.empty())
    {
      return;
    }
    out << "Missing characters (" << missing.size() << "): ";
    CharPrinter printer(out);
    std::for_each(missing.begin(), missing.end(), printer);
    out << "\n";
  }
}

void duhanina::build_codes(str_t input_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  if (encoding_store.count(encoding_id) > 0)
  {
    throw std::runtime_error("ID_EXISTS");
  }
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text((std::istreambuf_iterator< char >(in)), std::istreambuf_iterator< char >());
  CodeTable table = build_code_table(text);
  encoding_store[encoding_id] = table;
  out << "Code table successfully built and saved with ID '" << encoding_id << "'\n";
}

void duhanina::show_codes(str_t encoding_id, std::ostream& out)
{
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  const CodeTable& table = it->second;
  out << "Code table for " << encoding_id << ":\n";
  out << "Char\tCode\n";
  CodePairPrinter printer(out);
  std::for_each(table.char_to_code.begin(), table.char_to_code.end(), printer);
}

void duhanina::save_codes(str_t encoding_id, str_t output_file, std::ostream& out)
{
  validate_extension(output_file, CODE_TABLE_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  save_code_table(it->second, output_file);
  out << "Code table successfully saved to file '" << output_file << "'\n";
}

void duhanina::load_codes(str_t codes_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  if (encoding_store.count(encoding_id) > 0)
  {
    throw std::runtime_error("ID_EXISTS");
  }
  CodeTable table = load_code_table(codes_file);
  encoding_store[encoding_id] = table;
  out << "Code table successfully loaded from file '" << codes_file << "' with ID '" << encoding_id << "'\n";
}

void duhanina::clear_codes(str_t encoding_id, std::ostream& out)
{
  if (encoding_store.erase(encoding_id) > 0)
  {
    out << "Code table '" << encoding_id << "' successfully removed\n";
  }
  else
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
}


void duhanina::encode_file_with_codes(str_t input_file, str_t output_file, str_t codes_file, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  CodeTable table = load_code_table(codes_file);
  encode_file_impl(input_file, output_file, table, out);
}


void duhanina::decode_file_with_codes(str_t input_file, str_t output_file, str_t codes_file, std::ostream& out)
{
  validate_extension(codes_file, CODE_TABLE_EXT);
  validate_extension(input_file, COMPRESSED_EXT);
  validate_extension(output_file, TEXT_EXT);
  CodeTable table = load_code_table(codes_file);
  decode_file_impl(input_file, output_file, table, out);
}


void duhanina::encode_file(str_t input_file, str_t output_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  validate_extension(output_file, COMPRESSED_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  encode_file_impl(input_file, output_file, it->second, out);
}


void duhanina::decode_file(str_t input_file, str_t output_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, COMPRESSED_EXT);
  validate_extension(output_file, TEXT_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("NO_SUCH_ID");
  }
  decode_file_impl(input_file, output_file, it->second, out);
}

void duhanina::compare(str_t file1, str_t file2, str_t encod_id1, str_t encod_id2, std::ostream& out)
{
  validate_extension(file1, TEXT_EXT);
  validate_extension(file2, TEXT_EXT);
  if (file1 == file2)
  {
    throw std::runtime_error("IDENTICAL_TEXTS");
  }
  auto it1 = encoding_store.find(encod_id1);
  auto it2 = encoding_store.find(encod_id2);
  if (it1 == encoding_store.end() || it2 == encoding_store.end())
  {
     throw std::runtime_error("NO_SUCH_ID");
  }
  std::ifstream in1(file1), in2(file2);
  if (!in1 || !in2)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text1((std::istreambuf_iterator< char >(in1)), std::istreambuf_iterator< char >());
  std::string text2((std::istreambuf_iterator< char >(in2)), std::istreambuf_iterator< char >());
  if (text1 == text2)
  {
    throw std::runtime_error("IDENTICAL_TEXTS");
  }
  std::string encoded1 = encode_text(text1, it1->second);
  std::string encoded2 = encode_text(text2, it2->second);
  double size1_orig = text1.size();
  double size1_comp = std::ceil(encoded1.size() / 8.0) + sizeof(size_t);
  double ratio1 = size1_comp / size1_orig;
  double size2_orig = text2.size();
  double size2_comp = std::ceil(encoded2.size() / 8.0) + sizeof(size_t);
  double ratio2 = size2_comp / size2_orig;
  out << "Compression efficiency comparison:\n";
  out << "----------------------------------------\n";
  out << "File 1: " << file1 << " (encoding: " << encod_id1 << ")\n";
  out << "  Original size: " << size1_orig << " bytes\n";
  out << "  Compressed size: " << size1_comp << " bytes\n";
  out << "  Compression ratio: " << std::fixed << std::setprecision(2) << (ratio1 * 100) << "%\n";
  out << "----------------------------------------\n";
  out << "File 2: " << file2 << " (encoding: " << encod_id2 << ")\n";
  out << "  Original size: " << size2_orig << " bytes\n";
  out << "  Compressed size: " << size2_comp << " bytes\n";
  out << "  Compression ratio: " << std::fixed << std::setprecision(2) << (ratio2 * 100) << "%\n";
  out << "----------------------------------------\n";
  out << "Difference in compression ratio: " << std::fixed << std::setprecision(2) << std::fabs(ratio1 - ratio2) * 100 << "%\n";
}


void duhanina::list_encodings(std::ostream& out)
{
  if (encoding_store.empty())
  {
    out << "No saved encodings available\n";
    return;
  }
  out << "Available encodings:\n";
  EncodingInfoPrinter printer(out);
  std::for_each(encoding_store.begin(), encoding_store.end(), printer);
}


void duhanina::print_help(std::ostream& out)
{
  out << "Available commands:\n";
  out << "build_codes <input_file> <encoding_id> - build code table\n";
  out << "show_codes <encoding_id> - show code table\n";
  out << "save_codes <encoding_id> <output_file> - save code table\n";
  out << "load_codes <input_file> <encoding_id> - load code table\n";
  out << "clear_codes <encoding_id> - remove code table\n";
  out << "encode_file_with_codes <input> <output> <codes_file> - compress file\n";
  out << "decode_file_with_codes <input> <output> <codes_file> - decompress file\n";
  out << "encode_file <input> <output> <encoding_id> - compress file\n";
  out << "decode_file <input> <output> <encoding_id> - decompress file\n";
  out << "compare <file1> <file2> <encoding_id1> <encoding_id2> - compare efficiency\n";
  out << "list_encodings - list all encodings\n";
  out << "check_encoding <input_file> <encoding_id> - verify if encoding supports all file characters\n";
  out << "suggest_encodings <input_file> - show which encodings best support the file\n";
  out << "--help - show this help\n";
}

void duhanina::check_encoding(str_t input_file, str_t encoding_id, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  auto it = encoding_store.find(encoding_id);
  if (it == encoding_store.end())
  {
    throw std::runtime_error("Encoding '" + encoding_id + "' not found");
  }
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text((std::istreambuf_iterator< char >(in)), std::istreambuf_iterator< char >());
  auto missing = find_missing_chars(text, it->second.char_to_code);
  if (missing.empty())
  {
    out << "Encoding fully supports the file\n";
  }
  else
  {
    out << "Encoding issues in '" << encoding_id << "':\n";
    print_missing_chars(missing, out);
  }
}

void duhanina::suggest_encodings(str_t input_file, std::ostream& out)
{
  validate_extension(input_file, TEXT_EXT);
  std::ifstream in(input_file);
  if (!in)
  {
    throw std::runtime_error("FILE_NOT_FOUND");
  }
  std::string text((std::istreambuf_iterator< char >(in)), std::istreambuf_iterator< char >());
  out << "Encoding compatibility report:\n";
  EncodingChecker checker(text, out);
  std::for_each(encoding_store.begin(), encoding_store.end(), checker);
}
