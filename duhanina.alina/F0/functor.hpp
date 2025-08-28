#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include <bitset>
#include <fstream>
#include <set>
#include <iterator>
#include "shannonFano.hpp"

namespace duhanina
{
  using str_t = const std::string&;

  std::set< char > find_missing_chars(str_t text, const std::map< char, std::string >& char_to_code);

  struct Line
  {
  public:
    friend std::istream& operator>>(std::istream& is, Line& line)
    {
      if (!(is >> line.symbol_ >> line.content_))
      {
        is.setstate(std::ios::failbit);
      }
      return is;
    }
    friend struct LineProcessor;
    friend struct StreamProcessor;
    friend struct CodeLoader;

  private:
    char symbol_;
    std::string content_;
  };

  struct LineProcessor
  {
  public:
    explicit LineProcessor(CodeTable& table);
    void operator()(const struct Line& line) const;

  private:
    CodeTable& table_ref_;
  };

  struct CodeLoader
  {
  public:
    bool is_complete() const;
    void process(std::istream& in, size_t& lines_processed);
    CodeLoader(CodeTable& table, size_t& count);
    void operator()(const Line& line) const;

  private:
    CodeTable& table_ref_;
    size_t& loaded_count_;
  };

  struct NullChecker
  {
    bool operator()(const Node* node) const;
  };

  struct NodeMerger
  {
    Node* operator()(Node* subtree, Node* node) const;
  };

  struct FreqAccumulator
  {
    size_t operator()(size_t sum, Node* node) const;
  };

  struct SplitPositionFinder
  {
  public:
    explicit SplitPositionFinder(size_t half_total);
    bool operator()(Node* node);

  private:
    size_t sum_;
    const size_t half_total_;
  };

  struct NodeCreator
  {
    Node* operator()(const std::pair< char, size_t >& pair) const;
  };

  struct FreqCounter
  {
  public:
    explicit FreqCounter(std::map< char, size_t >& freq_map);
    void operator()(char c) const;

  private:
    std::map< char, size_t >& freq_map_;
  };

  struct CodeTableFiller
  {
  public:
    explicit CodeTableFiller(CodeTable& table);
    void operator()(const std::pair< char, std::string >& entry) const;

  private:
    CodeTable& table_;
  };

  struct FreqTransformer
  {
  public:
    explicit FreqTransformer(std::map< char, size_t >& freq_map);
    char operator()(char c) const;

  private:
    std::map< char, size_t >& freq_map_;
  };

  struct TableTransformer
  {
  public:
    explicit TableTransformer(CodeTable& table);
    void operator()(const std::pair< char, std::string >& entry) const;

  private:
    CodeTable& table_;
  };

  struct TreeBuilder
  {
  public:
    explicit TreeBuilder(std::vector< Node* >& nodes);
    void operator()() const;

  private:
    std::vector< Node* >& nodes_;
  };

  struct TreeDeleter
  {
    void operator()(Node* node) const;
  };

  struct CharEncoder
  {
  public:
    explicit CharEncoder(const duhanina::CodeTable& table, std::string& result);
    void operator()(char c) const;

  private:
    const CodeTable& table_;
    std::string& result_;
  };

  struct CodeAccumulator
  {
  public:
    CodeAccumulator(std::string& decoded, std::string& current_code, const CodeTable& table);
    void operator()(char bit);

  private:
    std::string& decoded_;
    std::string& current_code_;
    const CodeTable& table_;
  };

  struct SizeTByteWriter
  {
  public:
    explicit SizeTByteWriter(std::ofstream& out, size_t value);
    void operator()(size_t byte_index) const;

  private:
    std::ofstream& out_;
    size_t value_;
  };

  struct BitWriter
  {
  public:
    explicit BitWriter(std::ofstream& out);
    void operator()(char bit);
    void flush();

  private:
    std::ofstream& out_;
    char buffer_ = 0;
    size_t bit_pos_ = 0;
  };

  struct HeaderReader
  {
  public:
    HeaderReader(std::ifstream& in, size_t& bit_count);
    void operator()(int);

  private:
    std::ifstream& in_;
    size_t& bit_count_;
    size_t shift_;
  };

  struct DataProcessor
  {
  public:
    DataProcessor(size_t total_bits);
    void operator()(char byte);
    std::string get_result() const;
    bool is_complete() const;

  private:
    std::string result_;
    size_t total_bits_;
    size_t processed_;
  };

  struct BitHandler
  {
  public:
    BitHandler(std::string& result, size_t& processed, size_t total_bits);
    void operator()(size_t bit_pos);
    void set_bits(const std::bitset< 8 >& bits);

  private:
    std::string& result_;
    size_t& processed_;
    size_t total_bits_;
    std::bitset< 8 > bits_;
  };

  class TableEntryWriter
  {
  public:
    explicit TableEntryWriter(std::ofstream& output_stream);
    void operator()(const std::pair< char, std::string >& entry);

  private:
    std::ofstream& out_;
  };

  struct StreamProcessor
  {
  public:
    void operator()(std::istream_iterator< std::string > it) const;
    explicit StreamProcessor(CodeTable& table);
    void operator()(const std::string& str);
    void operator()(const Line& line) const;

  private:
    CodeTable& table_ref;
    LineProcessor line_processor_;
    void process_element(const std::string& str) const;
  };

  struct CharChecker
  {
  public:
    CharChecker(const std::map< char, std::string >& char_map);
    bool operator()(char c) const;

  private:
    const std::map< char, std::string >& char_to_code_ref_;
  };

  struct CharInserter
  {
  public:
    CharInserter(std::set< char >& missing_set);
    void operator()(char c) const;

  private:
    std::set< char >& missing_ref_;
  };

  struct CharPrinter
  {
  public:
    explicit CharPrinter(std::ostream& output_stream);
    void operator()(char c) const;

  private:
    std::ostream& out_;
  };

  struct CodePairPrinter
  {
  public:
    explicit CodePairPrinter(std::ostream& output_stream);
    void operator()(const std::pair< char, std::string >& char_code_pair) const;

  private:
    std::ostream& out_;
  };

  struct EncodingInfoPrinter
  {
  public:
    explicit EncodingInfoPrinter(std::ostream& output_stream);
    void operator()(const std::pair< str_t, CodeTable >& encoding_entry) const;

  private:
    std::ostream& out_;
  };

  struct EncodingChecker
  {
  public:
    EncodingChecker(const std::string& text, std::ostream& output_stream);
    void operator()(const std::pair< str_t, CodeTable >& encoding_pair) const;

  private:
    str_t text_ref_;
    std::ostream& out_;
  };

  template < size_t N = sizeof(size_t) >
  struct SizeTWriter
  {
    static void write(std::ofstream& out, size_t value)
    {
      out.put(static_cast< char >((value >> (8 * (sizeof(size_t) - N))) & 0xFF));
      SizeTWriter< N - 1 >::write(out, value);
    }
  };

  template <>
  struct SizeTWriter< 0 >
  {
    static void write(std::ofstream&, size_t){}
  };

  void write_size_t(std::ofstream& out, size_t value);
}

#endif
