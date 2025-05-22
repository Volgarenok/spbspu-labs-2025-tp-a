#ifndef FUNCTORS
#define FUNCTORS
#include <unordered_map>
#include <string>
#include "FrequencyDictionary.hpp"

namespace shapkov
{
  struct isAnagram
  {
    std::string& wrd;
    bool operator()(const std::string& word) const;
  };
  struct isPalindrome
  {
    size_t palindromesCnt = 0;
    bool operator()(const std::string& word);
  };
  void cleanWord(std::string& word);
  struct EntropyCalc
  {
    size_t size;
    double operator()(const std::pair< std::string, size_t >& word_pair) const;
  };
  struct ExtractFirst
  {
    std::string operator()(const std::pair< std::string, size_t >& p) const;
  };
  struct isPattern
  {
    std::string& pattern;
    size_t patternMatches = 0;
    bool operator()(const std::string& word);
  };
  bool CompareByFreq(const std::pair< std::string, size_t >& p1, const std::pair< std::string, size_t >& p2);
  struct PairPrinter
  {
    std::ostream& out;
    void operator()(const std::pair< std::string, size_t >& p) const;
  };
  struct ProcessWordPair
  {
    std::ostream& out;
    const std::string& dict_name;
    isAnagram& checker;
    size_t& counter;
    void operator()(const std::pair< std::string, size_t >& word_pair) const;
  };
  struct ProcessDictPair
  {
    std::ostream& out;
    isAnagram& checker;
    size_t& counter;
    void operator()(const std::pair< std::string, OneFreqDict >& dict_pair) const;
  };
  struct CheckFrequency
  {
    size_t target_freq;
    int delta;
    std::ostream& out;
    const std::string& dict_name;
    size_t& counter;
    void operator()(const std::pair< std::string, size_t >& word_pair) const;
  };
  struct ProcessDictionary
  {
    std::ostream& out;
    const std::string& word;
    int delta;
    size_t& counter;
    void operator()(const std::pair< std::string, OneFreqDict >& dict_pair) const;
  };
}

#endif
