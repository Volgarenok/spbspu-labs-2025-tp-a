#include "word_functors.hpp"
#include "index.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>

trukhanov::FindWord::FindWord(
  const ConcordanceIndex& index_,
  const std::string& word_,
  const std::string& indexName_,
  std::ostream& out_):
  word(word_), index(index_), indexName(indexName_), out(out_)
{}

void trukhanov::FindWord::operator()() const
{
  auto it = index.index.find(word);

  if (it == index.index.end())
  {
    out << "<NOT FOUND>\n";
    return;
  }

  WordEntry entry(*it);
  out << entry << '\n';
}

void trukhanov::ReplaceWordFunctor::operator()()
{
  if (oldWord == newWord)
  {
    return;
  }

  auto it = index.index.find(oldWord);

  if (it == index.index.end())
  {
    return;
  }

  index.index[newWord].insert(it->second.begin(), it->second.end());
  index.index.erase(oldWord);
}

trukhanov::ExportWordFunctor::ExportWordFunctor(
  const std::vector< std::string >& lines,
  std::ostream& out,
  const std::string& word):
  lines_(lines), out_(out), word_(word)
{}

void trukhanov::ExportWordFunctor::operator()(const IndexMap& index) const
{
  auto it = index.find(word_);
  if (it == index.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  WordEntry entry(*it);
  out_ << entry << '\n';
}

bool trukhanov::lengthGreater(const std::string& a, const std::string& b)
{
  return a.length() > b.length() || (a.length() == b.length() && a < b);
}

bool trukhanov::lengthLess(const std::string& a, const std::string& b)
{
  return a.length() < b.length() || (a.length() == b.length() && a < b);
}

std::string trukhanov::extractWord(const std::pair< const std::string, std::set< std::size_t > >& pair)
{
  return pair.first;
}

std::string trukhanov::OutputWord::operator()(const std::string& word) const
{
  return word;
}

std::pair< std::string, std::size_t > trukhanov::frequencyCollector(const IndexMap::value_type& entry)
{
  return { entry.first, entry.second.size() };
}

bool trukhanov::FindDifferentFrequencies::operator()(const std::string& word) const
{
  const auto& set1 = index1.index.at(word);
  const auto& set2 = index2.index.at(word);
  return set1.size() != set2.size();
}

bool trukhanov::compareByFrequencyDesc(
  const std::pair< std::string, std::size_t >& a,
  const std::pair< std::string, std::size_t >& b
)
{
  return a.second > b.second;
}
