#include "huffman.hpp"

namespace
{
  struct CharToPair
  {
    mazitov::freqTable& table;
    explicit CharToPair(mazitov::freqTable& t):
      table(t)
    {}
    std::pair< char, std::size_t > operator()(char c) const
    {
      return std::make_pair(c, ++table[c]);
    }
  };

  void treverseTree(const mazitov::huffPtr& node, const std::string& code, mazitov::huffCodesTable& codes)
  {
    if (node == nullptr)
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
      treverseTree(node->left, code + "0", codes);
    }
    if (node->right != nullptr)
    {
      treverseTree(node->right, code + "1", codes);
    }
  }
}

mazitov::HuffmanNode::HuffmanNode(std::size_t freq, char symb):
  frequency(freq),
  symbol(symb),
  left(nullptr),
  right(nullptr)
{}

mazitov::HuffmanNode::HuffmanNode(const huffPtr& l, const huffPtr& r):
  frequency(0),
  symbol('\0'),
  left(l),
  right(r)
{}

bool mazitov::HuffmanNodeComparator::operator()(const huffPtr& lhs, const huffPtr& rhs) const
{
  return lhs->frequency > rhs->frequency;
}

mazitov::freqTable mazitov::buildFreqTable(const std::string& text)
{
  freqTable ft;
  std::transform(text.begin(), text.end(), std::inserter(ft, ft.end()), CharToPair(ft));
  return ft;
}

mazitov::huffPtr mazitov::buildHuffmanTree(const freqTable& freq)
{
  if (freq.empty())
  {
    return nullptr;
  }

  std::priority_queue< huffPtr, std::vector< huffPtr >, HuffmanNodeComparator > pq;
  for (const auto& pair : freq)
  {
    pq.push(std::make_shared< HuffmanNode >(pair.second, pair.first));
  }

  while (pq.size() > 1)
  {
    auto left = pq.top();
    pq.pop();
    auto right = pq.top();
    pq.pop();
    auto parent = std::make_shared< HuffmanNode >(left, right);
    parent->frequency = left->frequency + right->frequency;
    pq.push(parent);
  }
  return pq.top();
}

void mazitov::generateCodes(const huffPtr& root, std::unordered_map< char, std::string >& codes)
{
  if (root == nullptr)
  {
    return;
  }
  treverseTree(root, "", codes);
}
