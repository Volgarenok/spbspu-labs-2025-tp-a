#ifndef FREQUENCY_DICTIONARY
#define FREQUENCY_DICTIONARY
#include <unordered_map>
#include <string>
#include <iostream>

namespace shapkov
{
  struct OneFreqDict
  {
    std::unordered_map< std::string, size_t > dictionary;
    size_t size = 0;
  };
  struct FrequencyDictionary
  {
    std::unordered_map< std::string, OneFreqDict > dicts;
  };
  void analyzeText(std::istream& in, std::ostream& out, FrequencyDictionary& dict);
  void wordInfo(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void anagrams(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void similarFrequency(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  double entropyCount(const OneFreqDict& text);
  void entropy(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void totalWords(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void palindromes(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void showWithPattern(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void print(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  std::string topWord(const OneFreqDict& text);
  void top(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void compare(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void intersect(std::istream& in, std::ostream& out, FrequencyDictionary& dict);
  void diff(std::istream& in, std::ostream& out, FrequencyDictionary& dict);
  void merge(std::istream& in, std::ostream& out, FrequencyDictionary& dict);
  void save(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
  void load(std::ostream& out, const std::string& fileName, FrequencyDictionary& dict);
}
#endif
