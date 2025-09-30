#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP
#include "dictionary.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cctype>

namespace shiryaeva
{
  char tolower_char(unsigned char c);
  std::string normalize_word(const std::string &s);

  struct RelativeFreqFilter
  {
    double min_rel, max_rel;
    std::size_t total_words;
    bool operator()(const std::pair<const std::string, std::size_t> &p) const;
  };

  struct ExcludeFreqFilter
  {
    double min_rel, max_rel;
    std::size_t total_words;
    bool operator()(const std::pair<const std::string, std::size_t> &p) const;
  };
  
  struct MinFreqFilter
  {
    size_t minf;
    bool operator()(const std::pair< const std::string, size_t > &p) const;
  };

  struct MaxFreqFilter
  {
    size_t maxf;
    std::string operator()(const std::pair< const std::string, size_t > &p) const;
  };

  struct Cmp
  {
    bool operator()(const std::pair< std::string, size_t >& a, const std::pair< std::string, size_t >& b) const;
  };

  struct Printer
  {
    std::string operator()(const std::pair< std::string, size_t >& p) const;
  };

  struct AddFreq
  {
    size_t operator()(size_t acc, const std::pair< const std::string, size_t >& p) const;
  };

  struct Adder
  {
    FrequencyDictionary &d;
    void operator()(const std::string &w) const;
  };

  struct GetKey
  {
    std::string operator()(const std::pair< const std::string, size_t >& p) const;
  };

  struct ProcessWord
  {
    FrequencyDictionary &dict;
    std::string operator()(const std::string& w) const;
  };

  struct MergeWord
  {
    FrequencyDictionary &target;
    const FrequencyDictionary &source;
    std::string operator()(const std::string& word) const;
  };

  struct RemoveWord
  {
    FrequencyDictionary &dict;
    std::string operator()(const std::string& key) const;
  };
}

#endif
