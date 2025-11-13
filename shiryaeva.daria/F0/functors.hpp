#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP
#include "dictionary.hpp"
#include <string>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

namespace shiryaeva
{
  char tolower_char(unsigned char c);
  std::string normalize_word(const std::string &s);

  struct RelativeFreqFilter
  {
    double min_rel, max_rel;
    size_t total_words;
    bool operator()(const std::pair< const std::string, size_t > &p) const;
  };

  struct ExcludeFreqFilter
  {
    double min_rel, max_rel;
    size_t total_words;
    bool operator()(const std::pair< const std::string, size_t > &p) const;
  };

  struct Cmp
  {
    bool operator()(const std::pair< std::string, size_t >& a, const std::pair< std::string, size_t >& b) const;
  };

  struct Printer
  {
    std::string operator()(const std::pair< std::string, size_t >& p) const;
  };

  struct WordInserter
  {
    using value_type = std::string;
    FrequencyDictionary &dict;
    void push_back(const std::string& word);
  };

  struct PrinterInserter
  {
    using value_type = std::pair< std::string, size_t >;
    std::ostream &out;
    void push_back(const std::pair< std::string, size_t >& pair);
  };

  struct MergeInserter
  {
    using value_type = std::pair< const std::string, size_t >;
    FrequencyDictionary &target;
    void push_back(const std::pair< const std::string, size_t >& pair);
  };
}

#endif
