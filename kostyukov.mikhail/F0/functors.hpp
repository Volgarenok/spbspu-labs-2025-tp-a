#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include "freqDict.hpp"

namespace kostyukov
{
  struct IsInvalidChar
  {
    bool operator()(char c) const;
  };

  struct IsPunct
  {
    bool operator()(const char c) const;
  };

  struct WordProcessor
  {
    explicit WordProcessor(FrequencyDictionary& dict);
    void operator()(std::string word);
    private:
      FrequencyDictionary& dict_;
  };

  struct MapToPairTransformer
  {
    MapToPairTransformer(size_t totalWords);
    WordFreqPair operator()(const std::pair< const std::string, size_t >& mapPair) const;
    private:
      size_t totalWords_;
  };

  struct FreqComparator
  {
    explicit FreqComparator(bool ascending = false);
    bool operator()(const WordFreqPair& first, const WordFreqPair& second) const;
    private:
      bool ascending_;
  };

  struct PairPrinter
  {
    explicit PairPrinter(std::ostream& out);
    void operator()(const WordFreqPair&) const;
    private:
      std::ostream& out_;
  };

  struct KeyExtractor
  {
    std::string operator()(const std::pair< const std::string, size_t >& mapPair) const;
  };

  struct RemoveBatchPredicate
  {
    RemoveBatchPredicate(size_t total, bool less, double n);
    bool operator()(const std::pair< const std::string, size_t >& pair) const;
    private:
      size_t totalWords_;
      bool isLess_;
      double threshold_;
  };

  struct DictDeleter
  {
    explicit DictDeleter(FrequencyDictionary& dict);
    void operator()(const std::pair< const std::string, size_t >& pairToRemove) const;
    private:
      FrequencyDictionary& dict_;
  };

  struct SameWordPrinter
  {
    std::ostream& out;
    const FrequencyDictionary& dict1;
    const FrequencyDictionary& dict2;
    const std::string& name1;
    const std::string& name2;
    void operator()(const std::string& key) const;
  };

  struct RangeFreqPredicate
  {
    RangeFreqPredicate(double minVal, double maxVal);
    bool operator()(const WordFreqPair& pair) const;
    private:
      double minVal_;
      double maxVal_;
  };

  struct OutOfRangeFreqPredicate
  {
    OutOfRangeFreqPredicate(double minVal, double maxVal);
    bool operator()(const WordFreqPair& pair) const;
    private:
      double minVal_;
      double maxVal_;
  };

  struct DictNamePrinter
  {
    std::ostream& out;
    void operator()(const std::pair< const std::string, FrequencyDictionary >& dictPair) const;
  };

  struct MergeFunctor
  {
    explicit MergeFunctor(FrequencyDictionary& targetDict);
    void operator()(const std::pair< const std::string, size_t >& pair);
    private:
      FrequencyDictionary& targetDict_;
  };

  struct IntersectFunctor
  {
    IntersectFunctor(FrequencyDictionary& targetDict, const FrequencyDictionary& otherDict, bool useMax);
    void operator()(const std::pair< const std::string, size_t >& pair);
    private:
      FrequencyDictionary& targetDict_;
      const FrequencyDictionary& otherDict_;
      bool useMax_;
  };
}
#endif
