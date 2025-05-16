#include "functors.hpp"
#include <algorithm>
#include <regex>

shapkov::isAnagram::isAnagram(const std::string word)
{
  for (char c: word)
  {
    letterFreq[c]++;
  }
}

bool shapkov::isAnagram::operator()(const std::string word)
{
  std::unordered_map< char, size_t > srcLetterFreq;
  for (char c: word)
  {
    srcLetterFreq[c]++;
  }
  return srcLetterFreq.size() == letterFreq.size() && srcLetterFreq == letterFreq;
}

bool shapkov::isPalindrome(const std::string word)
{
  return std::equal(word.begin(), word.begin() + word.size() / 2, word.rbegin());
}

void shapkov::cleanWord(std::string& word) {
  std::regex nonAlpha("[^a-zA-Z`]");
  word = std::regex_replace(word, nonAlpha, "");
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}
