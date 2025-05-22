#include "functors.hpp"
#include <algorithm>
#include <regex>
#include <cmath>

bool shapkov::isAnagram::operator()(const std::string& word) const
{
  return word.size() == wrd.size() && std::is_permutation(word.begin(), word.end(), wrd.begin());
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

double shapkov::EntropyCalc::operator()(const std::pair< std::string, size_t >& word_pair) const
{
  double wordProbability = static_cast< double >(word_pair.second) / size;
  return wordProbability * log2(wordProbability);
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

void shapkov::PairPrinter::operator()(const std::pair< std::string, size_t >& p) const
{
  out << p.first << ": " << p.second << '\n';
}

void shapkov::ProcessWordPair::operator()(const std::pair< std::string, size_t >& word_pair) const
{
  if (checker(word_pair.first))
  {
    out << dict_name << ": word - " << word_pair.first << '\n';
    counter++;
  }
}

void shapkov::ProcessDictPair::operator()(const std::pair< std::string, OneFreqDict >& dict_pair) const
{
  ProcessWordPair word_processor{ out, dict_pair.first, checker, counter };
  std::for_each(dict_pair.second.dictionary.begin(), dict_pair.second.dictionary.end(), word_processor);
}

void shapkov::CheckFrequency::operator()(const std::pair< std::string, size_t >& word_pair) const
{
  if ((word_pair.second <= target_freq + delta) && (static_cast< int >(word_pair.second) >= static_cast< int >(target_freq) - delta))
  {
    out << dict_name << ": " << word_pair.first << " - " << word_pair.second << '\n';
    counter++;
  }
}

void shapkov::ProcessDictionary::operator()(const std::pair< std::string, OneFreqDict >& dict_pair) const
{
  auto wrd = dict_pair.second.dictionary.find(word);
  if (wrd != dict_pair.second.dictionary.end())
  {
    CheckFrequency checker{wrd->second, delta, out, dict_pair.first, counter};
    std::for_each(dict_pair.second.dictionary.begin(), dict_pair.second.dictionary.end(), checker);
  }
}
