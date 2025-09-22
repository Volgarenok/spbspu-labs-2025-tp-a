#ifndef HUFFMAN_CORE_HPP
#define HUFFMAN_CORE_HPP
#include <string>
#include <unordered_map>
#include <memory>
#include <queue>
#include <vector>
namespace nikonov {
  using str = std::string;
  struct HuffmanNode {
    char character;
    int frequency;
    std::unique_ptr< HuffmanNode > left;
    std::unique_ptr< HuffmanNode > right;
    HuffmanNode(char c, int freq);
  };

  class HuffmanCore {
  public:
    static std::unordered_map< char, std::string > buildCodes(const std::string& text);
    static std::string compress(const std::string& text, const std::unordered_map< char, std::string >& codes);
    static std::string decompress(const std::string& compressed, const std::unordered_map< std::string, char >& reverseCodes);
  private:
    static std::unordered_map< char, int > calculateFrequency(const std::string& text);
    static void buildCodeTable(HuffmanNode* node, const std::string& code, std::unordered_map< char, std::string >& codes);
  };
}
#endif
