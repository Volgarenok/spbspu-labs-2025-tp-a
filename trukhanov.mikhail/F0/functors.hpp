#ifndef FUNCTORS_HPP
#define FUNCTORS_HPP

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "index.hpp"

namespace trukhanov
{
  struct PrintLine
  {
    std::ostream& out;
    void operator()(std::size_t line) const
    {
      out << ' ' << line;
    }
  };

  struct InRange
  {
    std::size_t from;
    std::size_t to;
    bool operator()(std::size_t val) const
    {
      return val >= from && val <= to;
    }
  };

  struct AdjustOffset
  {
    std::size_t offset;
    std::size_t operator()(std::size_t val) const
    {
      return val - offset;
    }
  };

  struct LineSplitter
  {
    std::map< std::string, std::set< std::size_t > >& wordMap;
    std::size_t lineNumber;
    void operator()(const std::string& word) const;
  };

  struct FrequencyCollector
  {
    std::vector< std::pair< std::string, std::size_t > >& result;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const;
  };

  struct ShowIndexEntry
  {
    std::ostream& out;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const;
  };

  struct ShowEntry
  {
    std::ostream& out;
    void operator()(const std::pair< std::string, std::size_t >& pair) const;
  };

  struct FindWord
  {
    const std::string& word;
    const ConcordanceIndex& index;
    const std::string& indexName;
    std::ostream& out;
    FindWord(const ConcordanceIndex& index, const std::string& word, const std::string& indexName, std::ostream& out);
    void operator()() const;
  };

  struct ReplaceWordFunctor
  {
    ConcordanceIndex& index;
    std::string oldWord;
    std::string newWord;
    void operator()();
  };

  struct LineFilter
  {
    std::size_t from;
    std::size_t to;
    const ConcordanceIndex& src;
    ConcordanceIndex& dest;
    LineFilter(
      const ConcordanceIndex& s,
      ConcordanceIndex& d,
      std::size_t f,
      std::size_t t) :
      from(f), to(t), src(s), dest(d)
    {
    }
    void operator()() const;
  };

  struct FileWriter
  {
    std::ostream& out;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& entry) const;
  };

  struct ExportWordFunctor
  {
    ExportWordFunctor(const std::vector< std::string >& lines_, std::ostream& out_, const std::string& word_);
    void operator()(const std::map< std::string, std::set< std::size_t > >& index) const;
  private:
    const std::vector< std::string >& lines;
    std::ostream& out;
    const std::string& word;
  };

  struct LengthGreater
  {
    bool operator()(const std::string& a, const std::string& b) const;
  };

  struct LengthLess
  {
    bool operator()(const std::string& a, const std::string& b) const;
  };

  struct ExtractWord
  {
    std::string operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const;
  };

  struct OutputWord
  {
    std::ostream& out;
    std::string operator()(const std::string& word) const;
  };

  struct FindDifferentFrequencies
  {
    const ConcordanceIndex& index1;
    const ConcordanceIndex& index2;
    bool operator()(const std::string& word) const;
  };

  struct InsertToIndex
  {
    ConcordanceIndex& index;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const;
  };

  struct MergeLinesFunctor
  {
    const std::vector< std::string >& lines1;
    const std::vector< std::string >& lines2;
    std::size_t i;
    std::string operator()();
  };

  struct CompareByFrequencyDesc
  {
    bool operator()(const std::pair< std::string, std::size_t >& a, const std::pair< std::string, std::size_t >& b) const;
  };

  struct FormatIndexEntry
  {
    std::string operator()(const std::pair< const std::string, ConcordanceIndex >& pair) const;
  };

  struct AddOffset
  {
    std::size_t offset;
    std::size_t operator()(std::size_t val) const
    {
      return val + offset;
    }
  };

  struct MergeWithOffset
  {
    trukhanov::ConcordanceIndex& dest;
    std::size_t offset;
    void operator()(const std::pair< const std::string, std::set< std::size_t > >& pair) const;
  };

  struct PrintExportLine
  {
    std::ostream& out;
    const std::vector< std::string >& lines;
    void operator()(std::size_t lineNo) const;
  };

  struct SplitWords
  {
    std::vector< std::string >& lines;
    std::map< std::string, std::set< std::size_t > >& wordMap;
    std::size_t operator()(std::size_t lineNumber) const;
  };
}

#endif
