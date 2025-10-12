#include "functors.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>

#include "scopeGuard.hpp"

bool kostyukov::IsInvalidChar::operator()(char c) const
{
  return !std::isalnum(c) && c != '_';
}

bool kostyukov::IsPunct::operator()(char c) const
{
  return std::ispunct(c);
}

kostyukov::WordProcessor::WordProcessor(FrequencyDictionary& dict):
  dict_(dict)
{}

void kostyukov::WordProcessor::operator()(std::string word)
{
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  word.erase(std::remove_if(word.begin(), word.end(), IsPunct{}), word.end());
  if (!word.empty())
  {
    dict_.counts[word]++;
    dict_.totalWords++;
  }
}

kostyukov::MapToPairTransformer::MapToPairTransformer(size_t totalWords):
  totalWords_(totalWords)
{}

kostyukov::WordFreqPair kostyukov::MapToPairTransformer::operator()(const std::pair< const std::string, size_t >& mapPair) const
{
  double freq = (totalWords_ == 0) ? 0.0 : (static_cast< double >(mapPair.second) / totalWords_) * 100.0;
  return WordFreqPair{ mapPair.first, freq };
}

kostyukov::FreqComparator::FreqComparator(bool ascending):
  ascending_(ascending)
{}

bool kostyukov::FreqComparator::operator()(const WordFreqPair& first, const WordFreqPair& second) const
{
  return (ascending_) ? first.freq < second.freq : first.freq > second.freq;
}

kostyukov::PairPrinter::PairPrinter(std::ostream& out):
  out_(out)
{}

void kostyukov::PairPrinter::operator()(const WordFreqPair& pair) const
{
  ScopeGuard scopeGrd(out_);
  out_ << pair.word << " - " << std::fixed << std::setprecision(1) << pair.freq << "%\n";
}

kostyukov::RemoveBatchPredicate::RemoveBatchPredicate(size_t total, bool less, double n):
  totalWords_(total),
  isLess_(less),
  threshold_(n)
{}

bool kostyukov::RemoveBatchPredicate::operator()(const std::pair< const std::string, size_t >& pair) const
{
  if (totalWords_ == 0)
  {
    return false;
  }
  double freq = (static_cast< double >(pair.second) / totalWords_) * 100.0;
  return isLess_ ? (freq < threshold_) : (freq > threshold_);
}

kostyukov::DictDeleter::DictDeleter(FrequencyDictionary& dict):
  dict_(dict)
{}

void kostyukov::DictDeleter::operator()(const std::pair< const std::string, size_t >& pairToRemove) const
{
  dict_.totalWords -= pairToRemove.second;
  dict_.counts.erase(pairToRemove.first);
}

std::string kostyukov::KeyExtractor::operator()(const std::pair< const std::string, size_t >& mapPair) const
{
  return mapPair.first;
}

void kostyukov::SameWordPrinter::operator()(const std::string& key) const
{
  ScopeGuard scopeGrd(out);
  out << key << " (in " << name1 << ": " << dict1.counts.at(key) << ", in " << name2 << ": " << dict2.counts.at(key) << ")\n";
}

kostyukov::RangeFreqPredicate::RangeFreqPredicate(double minVal, double maxVal):
  minVal_(minVal),
  maxVal_(maxVal)
{}

bool kostyukov::RangeFreqPredicate::operator()(const WordFreqPair& pair) const
{
  return pair.freq >= minVal_ && pair.freq <= maxVal_;
}

kostyukov::OutOfRangeFreqPredicate::OutOfRangeFreqPredicate(double minVal, double maxVal):
  minVal_(minVal),
  maxVal_(maxVal)
{}

bool kostyukov::OutOfRangeFreqPredicate::operator()(const WordFreqPair& pair) const
{
  return pair.freq < minVal_ || pair.freq > maxVal_;
}

void kostyukov::DictNamePrinter::operator()(const std::pair< const std::string, FrequencyDictionary >& dictPair) const
{
  out << dictPair.first << '\n';
}

kostyukov::MergeFunctor::MergeFunctor(FrequencyDictionary& targetDict):
  targetDict_(targetDict)
{}

void kostyukov::MergeFunctor::operator()(const std::pair< const std::string, size_t >& pair)
{
  targetDict_.counts[pair.first] += pair.second;
  targetDict_.totalWords += pair.second;
}

kostyukov::IntersectFunctor::IntersectFunctor(FrequencyDictionary& targetDict, const FrequencyDictionary& otherDict, bool useMax):
  targetDict_(targetDict),
  otherDict_(otherDict),
  useMax_(useMax)
{}

void kostyukov::IntersectFunctor::operator()(const std::pair< const std::string, size_t >& pair)
{
  auto otherIt = otherDict_.counts.find(pair.first);
  if (otherIt != otherDict_.counts.end())
  {
    size_t freq1 = pair.second;
    size_t freq2 = otherIt->second;
    size_t newFreq = useMax_ ? std::max(freq1, freq2) : std::min(freq1, freq2);
    targetDict_.counts[pair.first] = newFreq;
    targetDict_.totalWords += newFreq;
  }
}
