#ifndef FUNCTORS
#define FUNCTORS
#include <unordered_map>
#include <string>

namespace shapkov
{
  struct isAnagram
  {
    isAnagram(const std::string word);
    bool operator()(const std::string word);
    std::unordered_map< char, size_t > letterFreq;
  };
  bool isPalindrome(const std::string word);
  void cleanWord(std::string& word);
}

#endif
