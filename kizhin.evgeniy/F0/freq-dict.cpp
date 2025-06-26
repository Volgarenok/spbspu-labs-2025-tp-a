#include "freq-dict.hpp"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>

namespace kizhin {
  void appendWord(FrequencyDictionary&, const std::string&);
}

bool kizhin::SizeDescendingComp::operator()(const WordAndSize& lhs,
    const WordAndSize& rhs) const
{
  if (lhs.second != rhs.second) {
    return lhs.second > rhs.second;
  }
  return lhs.first < rhs.first;
}

void kizhin::expandDictionary(std::istream& in, FrequencyDictionary& dict)
{
  using InIt = std::istream_iterator< std::string >;
  using std::placeholders::_1;
  const auto appender = std::bind(std::addressof(appendWord), std::ref(dict), _1);
  std::for_each(InIt{ in }, InIt{}, appender);
}

void kizhin::appendWord(FrequencyDictionary& dict, const std::string& word)
{
  WordMap& wordMap = dict.wordMap;
  WordSet& wordSet = dict.wordSet;
  SizeSet& sizeSet = dict.sizeSet;
  if (!wordMap.count(word)) {
    wordMap[word] = 0;
    wordSet.insert(word);
  }
  std::size_t& wordCount = wordMap.at(word);
  sizeSet.erase({ word, wordCount });
  sizeSet.insert({ word, ++wordCount });
  ++dict.total;
}

