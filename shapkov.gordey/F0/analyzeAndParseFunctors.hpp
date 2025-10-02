#ifndef ANALYZE_PARSE_FUNCTORS
#define ANALYZE_PARSE_FUNCTORS
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>
#include "FrequencyDictionary.hpp"

namespace shapkov
{
  struct isAnagram
  {
    std::string& wrd;
    bool operator()(const std::pair< std::string, size_t >& wordPair) const;
  };
  struct FrequencyChecker
  {
    std::string& word;
    int delta;
    std::ostream& out;
    size_t targetFreq;
    bool operator()(const std::pair< std::string, size_t >& wordPair) const;
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
    double operator()(const std::pair< std::string, size_t >& wordPair) const;
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
  struct PairIO
  {
    const std::pair< std::string, size_t >& pair;
  };
  std::ostream& operator<<(std::ostream& out, const PairIO& p);
  PairIO makePairIO(const std::pair< std::string, size_t >& p);
  template< class Cond >
  class ConditionPrinter
  {
  public:
    ConditionPrinter(std::ostream& out, const std::string& dictName, Cond& checker, size_t& counter):
      out_(out),
      dictName_(dictName),
      checker_(checker),
      counter_(counter)
    {}
    void operator()(const std::pair< std::string, size_t >& wordPair) const
    {
      if (checker_(wordPair))
      {
        out_ << dictName_ << ": " << wordPair.first << " - " << wordPair.second << '\n';
        counter_++;
      }
    }
  private:
    std::ostream& out_;
    const std::string& dictName_;
    Cond& checker_;
    size_t& counter_;
  };
  template< class Cond >
  class DictProcessor
  {
  public:
    DictProcessor(std::ostream& out, Cond& checker, size_t& counter):
      out_(out),
      checker_(checker),
      counter_(counter)
    {}
    void operator()(const std::pair< std::string, OneFreqDict >& dictPair) const
    {
      ConditionPrinter< Cond > printer{ out_, dictPair.first, checker_, counter_ };
      std::for_each(dictPair.second.dictionary.begin(), dictPair.second.dictionary.end(), printer);
    }
  private:
    std::ostream& out_;
    Cond& checker_;
    size_t& counter_;
  };
  template<>
  class DictProcessor< FrequencyChecker >
  {
  public:
    DictProcessor(std::ostream& out, FrequencyChecker& checker, size_t& counter):
      out_(out),
      checker_(checker),
      counter_(counter)
    {}
    void operator()(const std::pair<std::string, OneFreqDict>& dictPair) const
    {
      auto wrd = dictPair.second.dictionary.find(checker_.word);
      if (wrd != dictPair.second.dictionary.end())
      {
        checker_.targetFreq = wrd->second;
        ConditionPrinter< FrequencyChecker > printer(out_, dictPair.first, checker_, counter_);
        std::for_each(dictPair.second.dictionary.begin(), dictPair.second.dictionary.end(), printer);
      }
    }
  private:
    std::ostream& out_;
    FrequencyChecker& checker_;
    size_t& counter_;
  };
  struct MergeFunctor
  {
    std::unordered_map< std::string, size_t >& dest;
    size_t& size;
    void operator()(const std::pair< const std::string, size_t >& p) const;
  };
  struct DiffFunctor
  {
    const std::unordered_map< std::string, size_t >& reference;
    std::unordered_map< std::string, size_t >& result;
    size_t& size;
    void operator()(const std::pair< const std::string, size_t >& p) const;
  };
  struct IntersectFunctor
  {
    const std::unordered_map< std::string, size_t >& reference;
    std::unordered_map< std::string, size_t >& result;
    size_t& size;
    void operator()(const std::pair< const std::string, size_t >& p) const;
  };
}

#endif
