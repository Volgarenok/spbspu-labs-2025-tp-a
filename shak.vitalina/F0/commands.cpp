#include "commands.hpp"
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iomanip>
#include <vector>

void shak::createDict(shak::Dictionaries &dicts, std::istream &in)
{
  std::string dictName;
  in >> dictName;
  if (!in || dictName.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  if (dicts.count(dictName))
  {
    throw std::invalid_argument("dictionary with this name exists");
  }
  dicts.emplace(dictName, FrequencyDictionary{});
}

void shak::clearDict(shak::Dictionaries &dicts, std::istream &in)
{
  std::string dictName;
  in >> dictName;
  if (!in || dictName.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter = dicts.find(dictName);
  if (dictIter == dicts.end())
  {
    throw std::invalid_argument("dictionary not found");
  }
  dictIter->second.dictionary.clear();
}

void shak::deleteDict(shak::Dictionaries &dicts, std::istream &in)
{
  std::string dictName;
  in >> dictName;
  if (!in || dictName.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter = dicts.find(dictName);
  if (dictIter == dicts.end())
  {
    throw std::invalid_argument("dictionary not found");
  }
  dicts.erase(dictIter);
}

void shak::printDict(Dictionaries& dicts, std::istream& in, std::ostream& out)
{
  std::string dictName;
  in >> dictName;
  if (dictName.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto it = dicts.find(dictName);
  if (it == dicts.end())
  {
    throw std::invalid_argument("no dictionary with name");
  }
  const auto &dictIter = it->second.dictionary;
  using outIt = std::ostream_iterator<std::string>;
  std::transform(dictIter.begin(), dictIter.end(), outIt(out, "\n"), printPair);
}

void shak::insertW(Dictionaries &dicts, std::istream &in)
{
  std::string word;
  std::string dictName;
  in >> word >> dictName;
  auto dictIter = dicts.find(dictName);
  if (word == "" || dictName == "")
  {
    throw std::invalid_argument("no arguments");
  }
  if (dictIter == dicts.end())
  {
    throw std::invalid_argument("no dictionary with name");
  }
  else
  {
    if (dictIter->second.dictionary.find(word) == dicts.find(dictName)->second.dictionary.end())
    {
      dictIter->second.dictionary.insert(std::pair< std::string, size_t >(word, 1));
    }
    else
    {
      dictIter->second.dictionary.find(word)->second++;
    }
  }
}

void shak::removeW(Dictionaries &dicts, std::istream &in)
{
  std::string word;
  std::string dictName;
  in >> word >> dictName;
  auto dictIter = dicts.find(dictName);
  if (word == "" || dictName == "")
  {
    throw std::invalid_argument("no arguments");
  }
  if (dicts.find(dictName) == dicts.end())
  {
    throw std::invalid_argument("no dictionary with name");
  }
  else
  {
    if (dictIter->second.dictionary.find(word) == dictIter->second.dictionary.end())
    {
      throw std::invalid_argument("no word in dictionary");
    }
    else
    {
      dicts.find(dictName)->second.dictionary.erase(word);
    }
  }
}

void shak::getFreq(Dictionaries &dicts, std::istream &in, std::ostream &out)
{
  std::string word;
  std::string dictName;
  in >> word >> dictName;
  out << "getFreq";
  auto dictIter = dicts.find(dictName);
  if (word == "" || dictName == "")
  {
    throw std::invalid_argument("no arguments");
  }
  if (dictIter == dicts.end())
  {
    throw std::invalid_argument("no dictionary with name");
  }
  else
  {
    if (dictIter->second.dictionary.find(word) == dicts.find(dictName)->second.dictionary.end())
    {
      out << "0/n";
    }
    else
    {
      out << dicts.find(dictName)->second.dictionary.find(word)->second << '\n';
    }
  }
}

void shak::getUniqe(Dictionaries &dicts, std::istream& in, std::ostream& out)
{
  std::string dictName;
  in >> dictName;
  if (dictName.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter = dicts.find(dictName);
  if (dictIter == dicts.end())
  {
    throw std::invalid_argument("no dictionary with name");
  }
  auto &dictFreq = dictIter->second.dictionary;
  size_t count = std::count_if(dictFreq.begin(), dictFreq.end(), uniqueOnce);
  out << count << '\n';
}

void shak::mostPopular(Dictionaries &dicts, std::istream &in, std::ostream &out)
{
  std::string dictName;
  size_t n{};
  in >> dictName >> n;
  auto dictIter = dicts.find(dictName);
  if (dictIter == dicts.end()) {
    throw std::invalid_argument("no dictionaries with names");
  }
  const auto &dict = dictIter->second.dictionary;
  if (dict.empty()) {
    throw std::invalid_argument("dictionary is empty");
  }
  std::vector< std::pair< std::string, size_t > > words;
  words.reserve(dict.size());
  std::copy(dict.begin(), dict.end(), std::back_inserter(words));
  size_t num = n;
  if (n == 0 || n > words.size()) {
    num = words.size();
  }
  if (num < words.size()) {
    std::partial_sort(words.begin(), words.begin() + num, words.end(), sortByFreq);
  } else {
    std::sort(words.begin(), words.end(), sortByFreq);
  }
  std::transform(words.begin(), words.begin() + num, std::ostream_iterator<std::string>(out, "\n"), printPair);
}

void shak::equal(Dictionaries &dicts, std::istream &in, std::ostream &out)
{
  std::string dict1, dict2, key;
  in >> dict1 >> dict2 >> key;
  if (dict1.empty() || dict2.empty() || key.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter1 = dicts.find(dict1);
  auto dictIter2 = dicts.find(dict2);
  if (dictIter1 == dicts.end() || dictIter2 == dicts.end())
  {
    throw std::invalid_argument("no dictionaries with names");
  }
  auto word1 = dictIter1->second.dictionary.find(key);
  auto word2 = dictIter2->second.dictionary.find(key);
  if (word1 == dictIter1->second.dictionary.end() || word2 == dictIter2->second.dictionary.end())
  {
    throw std::invalid_argument("no such words in dictionaries");
  }
  if (word1->second == word2->second)
  {
    out << "YES\n";
  }
  else
  {
    out << "NO\n";
  }
}

void shak::compare(Dictionaries &dicts, std::istream &in, std::ostream &out)
{
  std::string dict1, dict2;
  in >> dict1 >> dict2;
  if (dict1.empty() || dict2.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter1 = dicts.find(dict1);
  auto dictIter2 = dicts.find(dict2);
  if (dictIter1 == dicts.end() || dictIter2 == dicts.end())
  {
    throw std::invalid_argument("no dictionaries with names");
  }
  if (dictIter1->second.dictionary.size() != dictIter2->second.dictionary.size())
  {
    out << 0 << '\n';
    return;
  }
  auto &d1 = dictIter1->second.dictionary;
  auto &d2 = dictIter2->second.dictionary;
  bool eq = std::equal(d1.begin(), d1.end(), d2.begin(), pairEqual);
  out << (eq ? 1 : 0) << '\n';
}

void shak::intersection(Dictionaries &dicts, std::istream &in)
{
  std::string dict1, dict2, dictNew;
  in >> dict1 >> dict2 >> dictNew;
  if (dict1.empty() || dict2.empty())
  {
    throw std::invalid_argument("no arguments");
  }
  auto dictIter1 = dicts.find(dict1);
  auto dictIter2 = dicts.find(dict2);
  if (dictIter1 == dicts.end() || dictIter2 == dicts.end() || dicts.count(dictNew))
  {
    throw std::invalid_argument("no dictionaries with names");
  }
  auto &d1 = dictIter1->second.dictionary;
  FrequencyDictionary newDict;
  std::copy_if(d1.begin(), d1.end(), std::inserter(newDict.dictionary, newDict.dictionary.begin()), IntersectDict(dictIter2->second));
  dicts.emplace(dictNew, std::move(newDict));
}
