#include "HuffmanCore.hpp"
#include <functional>

nikonov::HuffmanNode::HuffmanNode(char c, int freq):
  character(c),
  frequency(freq),
  left(nullptr),
  right(nullptr)
{}

std::unordered_map< char, int > nikonov::HuffmanCore::calculateFrequency(const str& text)
{
  std::unordered_map< char, int > freq;
  for (char c : text) {
    freq[c]++;
  }
  return freq;
}

void nikonov::HuffmanCore::buildCodeTable(HuffmanNode* node, const str& code, std::unordered_map< char, str >& codes)
{
  if (node->left == nullptr && node->right == nullptr) {
    codes[node->character] = code;
    return;
  }
  if (node->left != nullptr) {
    buildCodeTable(node->left.get(), code + "0", codes);
  }
  if (node->right != nullptr) {
    buildCodeTable(node->right.get(), code + "1", codes);
  }
}

std::unordered_map< char, std::string > nikonov::HuffmanCore::buildCodes(const str& text)
{
  if (text.empty()) {
    return {};
  }
  auto freq = calculateFrequency(text);
  auto compare = [](const HuffmanNode* a, const HuffmanNode* b) {
    return a->frequency > b->frequency;
  };
  std::priority_queue< HuffmanNode*, std::vector< HuffmanNode* >, decltype(compare) > pq(compare);

  for (const auto& pair : freq) {
    pq.push(new HuffmanNode(pair.first, pair.second));
  }
  while (pq.size() > 1) {
    HuffmanNode* left = pq.top();
    pq.pop();
    HuffmanNode* right = pq.top();
    pq.pop();
    auto parent = new HuffmanNode('\0', left->frequency + right->frequency);
    parent->left = std::unique_ptr< HuffmanNode >(left);
    parent->right = std::unique_ptr< HuffmanNode >(right);
    pq.push(parent);
  }

  HuffmanNode* root = pq.top();
  pq.pop();

  std::unordered_map< char, str > codes;
  if (freq.size() == 1) {
    codes[root->character] = "0";
  } else {
    buildCodeTable(root, "", codes);
  }

  delete root;
  return codes;
}

std::string nikonov::HuffmanCore::compress(const str& text, const std::unordered_map< char, str >& codes)
{
  str compressed;
  for (char c : text) {
    compressed += codes.at(c);
  }
  return compressed;
}

std::string nikonov::HuffmanCore::decompress(const str& compressed, const std::unordered_map< str, char >& reverseCodes)
{
  str decompressed;
  str currentCode;
  for (char bit : compressed) {
    currentCode += bit;
    if (reverseCodes.find(currentCode) != reverseCodes.end()) {
      decompressed += reverseCodes.at(currentCode);
      currentCode.clear();
    }
  }
  return decompressed;
}
