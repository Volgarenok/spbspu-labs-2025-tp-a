#include "output_functors.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

void trukhanov::ShowIndexEntry::operator()(const std::pair< const std::string, std::set< size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

void trukhanov::ShowEntry::operator()(const std::pair< std::string, std::size_t >& pair) const
{
  out << pair.first << ' ' << pair.second << '\n';
}

void trukhanov::FileWriter::operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const
{
  out << entry.first << ' ';
  std::copy(entry.second.begin(), entry.second.end(), std::ostream_iterator< std::size_t >(out, " "));
  out << '\n';
}

std::string trukhanov::MergeLinesFunctor::operator()()
{
  std::string line1;
  if (i < lines1.size())
  {
    line1 = lines1[i];
  }
  else
  {
    line1 = "";
  }

  std::string line2;
  if (i < lines2.size())
  {
    line2 = lines2[i];
  }
  else
  {
    line2 = "";
  }
  ++i;
  return line1 + " " + line2;
}

std::string trukhanov::FormatIndexEntry::operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const
{
  const std::string& file = pair.second.sourceFile;
  std::string result;

  if (file.empty())
  {
    result = pair.first + ": <NO FILE>";
  }
  else
  {
    result = pair.first + ": " + file;
  }
  return result;
}

std::size_t trukhanov::AddOffset::operator()(std::size_t val) const
{
  return val + offset;
}

void trukhanov::MergeWithOffset::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  std::set< std::size_t > adjusted;
  std::transform(
    pair.second.begin(),
    pair.second.end(),
    std::inserter(adjusted,adjusted.end()),
    trukhanov::AddOffset{ offset });
  dest.index[pair.first].insert(adjusted.begin(), adjusted.end());
}
