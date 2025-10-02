#include "functor.hpp"
#include <algorithm>
#include <limits>
#include <iomanip>
#include <numeric>
#include "shannonFano.hpp"

namespace
{
  using namespace duhanina;

  void delete_tree(Node* node)
  {
    if (node != nullptr)
    {
      delete_tree(node->left);
      delete_tree(node->right);
      delete node;
    }
  }

  Node* build_subtree(const std::vector< Node* >& nodes, size_t start, size_t end)
  {
    if (nodes.empty())
    {
      throw std::invalid_argument("Empty nodes list");
    }
    if (start >= end)
    {
      throw std::invalid_argument("Invalid range");
    }
    if (start >= nodes.size() || end > nodes.size())
    {
      throw std::out_of_range("Range exceeds nodes size");
    }
    if (std::any_of(nodes.begin() + start, nodes.begin() + end, NullChecker()))
    {
      throw std::invalid_argument("Null node");
    }
    Node* subtree = nullptr;
    try
    {
      subtree = new Node(*nodes[start]);
      subtree = std::accumulate(nodes.begin() + start + 1, nodes.begin() + end, subtree, NodeMerger());
    }
    catch (...)
    {
      delete_tree(subtree);
      throw;
    }
    return subtree;
  }

  std::pair< Node*, Node* > split_nodes(const std::vector< Node* >& nodes)
  {
    if (nodes.empty())
    {
      throw std::invalid_argument("Empty nodes vector");
    }
    if (std::any_of(nodes.begin(), nodes.end(), NullChecker()))
    {
      throw std::invalid_argument("Null node");
    }
    size_t total = std::accumulate(nodes.begin(), nodes.end(), 0, FreqAccumulator());
    SplitPositionFinder finder(total / 2);
    auto split_it = std::find_if(nodes.begin(), nodes.end(), std::ref(finder));
    size_t split_pos = std::distance(nodes.begin(), split_it);
    Node* left = nullptr;
    Node* right = nullptr;
    try
    {
      left = build_subtree(nodes, 0, split_pos);
      right = build_subtree(nodes, split_pos, nodes.size());
    }
    catch (...)
    {
      delete_tree(left);
      delete_tree(right);
      throw;
    }
    return { left, right };
  }

  std::vector< Node* > merge_nodes(Node* left, Node* right)
  {
    if (!left && !right)
    {
      return {};
    }
    if (!left)
    {
      return { right };
    }
    if (!right)
    {
      return { left };
    }
    Node* root = new Node(left->freq + right->freq, left, right);
    return { root };
  }
}

std::set< char > duhanina::find_missing_chars(str_t text, const std::map< char, std::string >& char_to_code)
{
  std::set< char > missing;
  CharChecker checker(char_to_code);
  CharInserter inserter(missing);
  std::copy_if(text.begin(), text.end(), std::inserter(missing, missing.begin()), std::ref(checker));
  return missing;
}

duhanina::LineProcessor::LineProcessor(CodeTable& table):
  table_ref_(table)
{}

void duhanina::LineProcessor::operator()(const Line& line) const
{
  const std::string& code = line.content_;
  const char sumb = line.symbol_;
  table_ref_.char_to_code[sumb] = code;
  table_ref_.code_to_char[code] = sumb;
}

duhanina::CodeLoader::CodeLoader(CodeTable& table, size_t& count):
  table_ref_(table),
  loaded_count_(count)
{}

void duhanina::CodeLoader::operator()(const Line& line) const
{
  LineProcessor linepr(table_ref_);
  linepr(line);
  loaded_count_++;
}

bool duhanina::CodeLoader::is_complete() const
{
  return loaded_count_ >= table_ref_.total_chars;
}

void duhanina::CodeLoader::process(std::istream& in, size_t& lines_processed)
{
  if (is_complete())
  {
    return;
  }
  Line line;
  if (in >> line)
  {
    (*this)(line);
    lines_processed++;
    process(in, lines_processed);
  }
}

bool duhanina::NullChecker::operator()(const duhanina::Node* node) const
{
  return node == nullptr;
}

duhanina::Node* duhanina::NodeMerger::operator()(duhanina::Node* subtree, duhanina::Node* node) const
{
  if (!subtree || !node)
  {
    throw std::invalid_argument("Null node");
  }
  Node* new_node = nullptr;
  Node* new_subtree = nullptr;
  try
  {
    new_node = new Node(*node);
    new_subtree = new Node(subtree->freq + new_node->freq, subtree, new_node);
  }
  catch (...)
  {
    delete new_subtree;
    delete new_node;
    throw;
  }
  return new_subtree;
}

size_t duhanina::FreqAccumulator::operator()(size_t sum, duhanina::Node* node) const
{
  return sum + node->freq;
}

duhanina::SplitPositionFinder::SplitPositionFinder(size_t half_total):
  sum_(0),
  half_total_(half_total)
{}

bool duhanina::SplitPositionFinder::operator()(duhanina::Node* node)
{
  sum_ += node->freq;
  return sum_ >= half_total_;
}

duhanina::Node* duhanina::NodeCreator::operator()(const std::pair< char, size_t >& pair) const
{
  return new Node(pair.first, pair.second);
}

duhanina::FreqCounter::FreqCounter(std::map< char, size_t >& freq_map):
  freq_map_(freq_map)
{}

void duhanina::FreqCounter::operator()(char c) const
{
  freq_map_[c]++;
}

duhanina::CodeTableFiller::CodeTableFiller(duhanina::CodeTable& table):
  table_(table)
{}

void duhanina::CodeTableFiller::operator()(const std::pair< char, std::string >& entry) const
{
  table_.code_to_char[entry.second] = entry.first;
}

duhanina::FreqTransformer::FreqTransformer(std::map< char, size_t >& freq_map):
  freq_map_(freq_map)
{}

char duhanina::FreqTransformer::operator()(char c) const
{
  freq_map_[c]++;
  return c;
}

duhanina::TableTransformer::TableTransformer(duhanina::CodeTable& table):
  table_(table)
{}

duhanina::TreeBuilder::TreeBuilder(std::vector< duhanina::Node* >& nodes):
  nodes_(nodes)
{}

void duhanina::TreeBuilder::operator()() const
{
  if (nodes_.size() <= 1)
  {
    return;
  }
  auto node_pair = split_nodes(nodes_);
  nodes_ = merge_nodes(node_pair.first, node_pair.second);
  (*this)();
}

void duhanina::TreeDeleter::operator()(duhanina::Node* node) const
{
  delete_tree(node);
}

duhanina::CharEncoder::CharEncoder(const duhanina::CodeTable& table, std::string& result):
  table_(table),
  result_(result)
{}

void duhanina::CharEncoder::operator()(char c) const
{
  auto it = table_.char_to_code.find(c);
  if (it == table_.char_to_code.end())
  {
    throw std::runtime_error("INVALID_CODES");
  }
  result_ += it->second;
}

duhanina::CodeAccumulator::CodeAccumulator(std::string& decoded, std::string& current_code, const duhanina::CodeTable& table):
  decoded_(decoded),
  current_code_(current_code),
  table_(table)
{}

void duhanina::CodeAccumulator::operator()(char bit)
{
  current_code_ += bit;
  auto it = table_.code_to_char.find(current_code_);
  if (it != table_.code_to_char.end())
  {
    decoded_ += it->second;
    current_code_.clear();
  }
}

void duhanina::TableTransformer::operator()(const std::pair< char, std::string >& entry) const
{
  table_.code_to_char[entry.second] = entry.first;
}

duhanina::SizeTByteWriter::SizeTByteWriter(std::ofstream& out, size_t value):
  out_(out),
  value_(value)
{}

void duhanina::SizeTByteWriter::operator()(size_t byte_index) const
{
  char byte = static_cast< char >((value_ >> (8 * byte_index)) & 0xFF);
  out_.put(byte);
}

duhanina::BitWriter::BitWriter(std::ofstream& out):
  out_(out)
{}

void duhanina::BitWriter::operator()(char bit)
{
  if (bit == '1')
  {
    buffer_ |= (1 << (7 - bit_pos_));
  }
  bit_pos_++;
  if (bit_pos_ == 8)
  {
    out_.put(buffer_);
    buffer_ = 0;
    bit_pos_ = 0;
  }
}

void duhanina::BitWriter::flush()
{
  if (bit_pos_ > 0)
  {
    out_.put(buffer_);
    buffer_ = 0;
    bit_pos_ = 0;
  }
}

duhanina::HeaderReader::HeaderReader(std::ifstream& in, size_t& bit_count):
  in_(in),
  bit_count_(bit_count),
  shift_(0)
{}

void duhanina::HeaderReader::operator()(int)
{
  char byte;
  if (!in_.get(byte))
  {
    throw std::runtime_error("INVALID_HEADER");
  }
  bit_count_ |= static_cast< size_t >(static_cast< unsigned char >(byte)) << (8 * shift_++);
}

duhanina::BitHandler::BitHandler(std::string& result, size_t& processed, size_t total_bits):
  result_(result),
  processed_(processed),
  total_bits_(total_bits)
{}

void duhanina::BitHandler::operator()(size_t bit_pos)
{
  if (processed_ < total_bits_)
  {
    if (bits_.test(7 - bit_pos))
    {
      result_ += '1';
    }
    else
    {
      result_ += '0';
    }
    ++processed_;
  }
}

void duhanina::BitHandler::set_bits(const std::bitset< 8 >& bits)
{
  bits_ = bits;
}

duhanina::DataProcessor::DataProcessor(size_t total_bits):
  total_bits_(total_bits),
  processed_(0)
{
  result_.reserve(total_bits);
}

void duhanina::DataProcessor::operator()(char byte)
{
  if (processed_ >= total_bits_)
  {
    return;
  }
  std::bitset< 8 > bits(byte);
  BitHandler handler(result_, processed_, total_bits_);
  handler.set_bits(std::bitset< 8 >(bits));
  std::vector< size_t > bit_positions(8);
  std::iota(bit_positions.begin(), bit_positions.end(), 0);
  std::for_each(bit_positions.begin(), bit_positions.end(), handler);
}

std::string duhanina::DataProcessor::get_result() const
{
  return result_;
}

bool duhanina::DataProcessor::is_complete() const
{
  return processed_ == total_bits_;
}

duhanina::TableEntryWriter::TableEntryWriter(std::ofstream& output_stream):
  out_(output_stream)
{}

void duhanina::TableEntryWriter::operator()(const std::pair< char, std::string >& entry)
{
  out_ << entry.first << " " << entry.second << "\n";
}

void duhanina::StreamProcessor::operator()(std::istream_iterator< std::string > it) const
{
  if (it != std::istream_iterator< std::string >())
  {
    process_element(*it);
    ++it;
  }
}

void duhanina::StreamProcessor::process_element(const std::string& str) const
{
  Line line;
  line.content_ = str;
  line_processor_(line);
}

duhanina::StreamProcessor::StreamProcessor(CodeTable& table):
  table_ref(table),
  line_processor_(table)
{}

void duhanina::StreamProcessor::operator()(const std::string& str)
{
  process_element(str);
}

void duhanina::StreamProcessor::operator()(const Line& line) const
{
  line_processor_(line);
}

duhanina::CharChecker::CharChecker(const std::map< char, std::string >& char_map):
  char_to_code_ref_(char_map)
{}

bool duhanina::CharChecker::operator()(char c) const
{
  return char_to_code_ref_.find(c) == char_to_code_ref_.end();
}

duhanina::CharInserter::CharInserter(std::set< char >& missing_set):
  missing_ref_(missing_set)
{}

void duhanina::CharInserter::operator()(char c) const
{
  missing_ref_.insert(c);
}

duhanina::CharPrinter::CharPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::CharPrinter::operator()(char c) const
{
  if (std::isprint(c))
  {
    out_ << "'" << c << "' ";
  }
  else
  {
    out_ << "[0x" << std::hex << static_cast< int >(c) << "] ";
  }
}

duhanina::CodePairPrinter::CodePairPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::CodePairPrinter::operator()(const std::pair< char, std::string >& char_code_pair) const
{
  const char c = char_code_pair.first;
  str_t code = char_code_pair.second;
  if (std::isprint(c) && !std::isspace(c))
  {
    out_ << "'" << c << "'\t" << code << "\n";
  }
  else
  {
    out_ << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast< int >(c) << std::dec << "\t" << code << "\n";
  }
}

duhanina::EncodingInfoPrinter::EncodingInfoPrinter(std::ostream& output_stream):
  out_(output_stream)
{}

void duhanina::EncodingInfoPrinter::operator()(const std::pair< str_t, CodeTable >& encoding_entry) const
{
  str_t encoding_id = encoding_entry.first;
  const CodeTable& table = encoding_entry.second;
  out_ << "  " << encoding_id << " (" << table.char_to_code.size() << " symbols, total " << table.total_chars << ")\n";
}

duhanina::EncodingChecker::EncodingChecker(str_t text, std::ostream& output_stream):
  text_ref_(text),
  out_(output_stream)
{}

void duhanina::EncodingChecker::operator()(const std::pair< str_t, CodeTable >& encoding_pair) const
{
  str_t id = encoding_pair.first;
  const CodeTable& table = encoding_pair.second;
  std::set< char > missing = find_missing_chars(text_ref_, table.char_to_code);
  out_ << " - " << id << ": ";
  if (missing.empty())
  {
    out_ << "FULL";
  }
  else
  {
    out_ << "partial";
  }
  out_ << " support\n";
}

void duhanina::write_size_t(std::ofstream& out, size_t value)
{
  SizeTWriter<>::write(out, value);
}
