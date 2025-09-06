#include "split_functors.hpp"
#include <algorithm>
#include <iterator>
#include <sstream>

void trukhanov::LineSplitter::operator()(const std::string& word) const
{
  wordMap[word].insert(lineNumber);
}

std::size_t trukhanov::SplitWords::operator()(std::size_t lineNumber) const
{
  const std::string& line = lines.at(lineNumber);
  std::istringstream iss(line);
  std::vector< std::string > words;
  std::copy(std::istream_iterator< std::string >(iss), std::istream_iterator< std::string >(), std::back_inserter(words));
  LineSplitter splitter{ wordMap, lineNumber + 1 };
  std::for_each(words.begin(), words.end(), splitter);
  return lineNumber;
}

void trukhanov::InsertToIndex::operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const
{
  index.index[pair.first] = pair.second;
}
