#ifndef SPLIT_FUNCTORS_HPP
#define SPLIT_FUNCTORS_HPP

#include <string>
#include <map>
#include <set>
#include "index.hpp"

namespace trukhanov
{
  struct LineSplitter
  {
    std::map< std::string, std::set< std::size_t > >& wordMap;
    std::size_t lineNumber;
    void operator()(const std::string& word) const;
  };

  struct SplitWords
  {
    std::vector< std::string >& lines;
    std::map< std::string, std::set< std::size_t > >& wordMap;
    std::size_t operator()(std::size_t lineNumber) const;
  };

  struct InsertToIndex
  {
    ConcordanceIndex& index;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const;
  };
}

#endif
