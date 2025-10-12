#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <string>
#include <queue>
#include <algorithm>

namespace mazitov
{
  struct HuffmanNode;
  using huffPtr = std::shared_ptr< HuffmanNode >;
  using freqTable = std::unordered_map< char, std::size_t >;
  using huffCodesTable = std::unordered_map< char, std::string >;

  struct HuffmanNode
  {
    std::size_t frequency;
    char symbol;
    huffPtr left;
    huffPtr right;

    HuffmanNode(std::size_t freq, char symb);
    HuffmanNode(const huffPtr &, const huffPtr &);
  };

  struct HuffmanNodeComparator
  {
    bool operator()(const huffPtr &, const huffPtr &) const;
  };

  huffPtr buildHuffmanTree(const freqTable &);
  freqTable buildFreqTable(const std::string &);
  void generateCodes(const huffPtr &, huffCodesTable &);
}

#endif
