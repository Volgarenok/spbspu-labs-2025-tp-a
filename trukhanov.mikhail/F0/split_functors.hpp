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
    IndexMap& wordMap;
    std::size_t lineNumber;
    std::string operator()(const std::string& word) const;
  };

  struct SplitWords
  {
    std::vector< std::string >& lines;
    IndexMap& wordMap;
    std::size_t operator()(std::size_t lineNumber) const;
  };

  bool isSpace(unsigned char ch);

  bool notSpace(unsigned char ch);

  struct WordCounter
  {
    bool prevSpase;
    WordCounter();
    bool operator()(unsigned char ch);
  };

  std::size_t countWords(const std::string& line);

  struct WordGenerator
  {
    std::string::const_iterator cur;
    std::string::const_iterator end;
    std::string operator()();
  };

  struct SplitAndAdd
  {
    ConcordanceIndex& idx;
    std::vector< std::string >& lines;
    std::size_t operator()(std::size_t lineNum) const;
  };
}

#endif
