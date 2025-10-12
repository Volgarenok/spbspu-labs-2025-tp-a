#include "analyzeAndParseFunctors.hpp"
#include <algorithm>
#include <cctype>
#include <regex>
#include <cmath>

bool shapkov::isAnagram::operator()(const std::pair< std::string, size_t >& wordPair) const
{
  const std::string& word = wordPair.first;
  return word.size() == wrd.size() && std::is_permutation(word.begin(), word.end(), wrd.begin());
}

bool shapkov::FrequencyChecker::operator()(const std::pair< std::string, size_t >& wordPair) const
{
  return (wordPair.second <= targetFreq + delta) && (static_cast< int >(wordPair.second) >= static_cast< int >(targetFreq) - delta);
}

bool shapkov::isPalindrome::operator()(const std::string& word)
{
  bool isPal = std::equal(word.begin(), word.begin() + word.size() / 2, word.rbegin());
  if (isPal)
  {
    palindromesCnt++;
  }
  return isPal;
}

void shapkov::cleanWord(std::string& word)
{
  std::regex nonAlpha("[^a-zA-Z`]");
  word = std::regex_replace(word, nonAlpha, "");
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

double shapkov::EntropyCalc::operator()(const std::pair< std::string, size_t >& wordPair) const
{
  double wordProbability = static_cast< double >(wordPair.second) / size;
  return wordProbability * std::log2(wordProbability);
}

std::string shapkov::ExtractFirst::operator()(const std::pair< std::string, size_t >& p) const
{
  return p.first;
}

bool shapkov::isPattern::operator()(const std::string& word)
{
  std::regex patt(pattern);
  bool isPatt = std::regex_match(word, patt);
  if (isPatt)
  {
    patternMatches++;
  }
  return isPatt;
}

bool shapkov::CompareByFreq(const std::pair< std::string, size_t >& p1, const std::pair< std::string, size_t >& p2)
{
  return p1.second < p2.second;
}

std::ostream& shapkov::operator<<(std::ostream& out, const PairIO& p)
{
  return out << p.pair.first << ": " << p.pair.second;
}

shapkov::PairIO shapkov::makePairIO(const std::pair< std::string, size_t >& p)
{
  return PairIO{ p };
}

void shapkov::MergeFunctor::operator()(const std::pair< const std::string, size_t >& p) const
{
  dest[p.first] += p.second;
  size += p.second;
}

void shapkov::DiffFunctor::operator()(const std::pair< const std::string, size_t >& p) const
{
  if (reference.find(p.first) == reference.end())
  {
    result[p.first] = p.second;
    size += p.second;
  }
}

void shapkov::IntersectFunctor::operator()(const std::pair< const std::string, size_t >& p) const
{
  auto it = reference.find(p.first);
  if (it != reference.end())
  {
    result[p.first] = p.second + it->second;
    size += p.second + it->second;
  }
}
