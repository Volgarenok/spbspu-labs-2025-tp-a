#include "word_functors.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

void trukhanov::FindWord::operator()() const
{
  auto it = index.index.find(word);

  if (it == index.index.end())
  {
    out << "<NOT FOUND>\n";
    return;
  }

  std::copy(it->second.begin(), it->second.end(), std::ostream_iterator< size_t >(out, " "));
  out << '\n';
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

void trukhanov::PrintExportLine::operator()(std::size_t lineNo) const
{
  if (lineNo == 0 || lineNo > lines.size())
  {
    out << " <INVALID LINE NUMBER>";
  }
  else
  {
    out << ' ' << lineNo;
  }
}

void trukhanov::ExportWordFunctor::operator()(const std::map< std::string, std::set< std::size_t > >& index) const
{
  auto it = index.find(word);

  if (it == index.end())
  {
    throw std::invalid_argument("Invalid command");
  }

  out << word;
  std::for_each(it->second.begin(), it->second.end(), trukhanov::PrintExportLine{ out, lines });
  out << '\n';
}

bool trukhanov::LengthGreater::operator()(const std::string& a, const std::string& b) const
{
  return a.length() > b.length() || (a.length() == b.length() && a < b);
}

bool trukhanov::LengthLess::operator()(const std::string& a, const std::string& b) const
{
  return a.length() < b.length() || (a.length() == b.length() && a < b);
}

std::string trukhanov::ExtractWord::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  return pair.first;
}

std::string trukhanov::OutputWord::operator()(const std::string& word) const
{
  return word;
}

void trukhanov::FrequencyCollector::operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const
{
  result.push_back(std::make_pair(entry.first, entry.second.size()));
}

bool trukhanov::FindDifferentFrequencies::operator()(const std::string& word) const
{
  const auto& set1 = index1.index.at(word);
  const auto& set2 = index2.index.at(word);
  return set1.size() != set2.size();
}

bool trukhanov::CompareByFrequencyDesc::operator()(
  const std::pair< std::string,
  std::size_t >& a,
  const std::pair< std::string,
  std::size_t >& b) const
{
  return a.second > b.second;
}
