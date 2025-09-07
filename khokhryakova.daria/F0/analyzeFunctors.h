#ifndef ANALYZE_FUNCTORS_HPP
#define ANALYZE_FUNCTORS_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "XrefDictionary.h"

namespace khokhryakova
{
  void cleanWord(std::string& word);

  struct ToLower
  {
    char operator()(char c) const;
  };

  struct WordExtractor
  {
    std::vector< std::string >& words;
    std::map< std::string, std::vector< Position > >& dictionary;
    size_t lineNum;
    mutable size_t colNum = 1;
    void operator()(const std::smatch& match) const;
  };

  struct TextWordExtractor
  {
    std::vector< std::string >& words;
    void operator()(const std::smatch& match) const;
  };

  struct PositionExtractor
  {
    std::vector< Position >& positions;
    void operator()(const std::smatch& match) const;
  };

  struct ExtractLine
  {
    std::vector< size_t >& lines;
    void operator()(const Position& pos);
  };

  struct PrintNumbers
  {
    std::ostream& out;
    mutable bool first = true;
    void operator()(size_t num) const;
  };

  struct PrintPair
  {
    std::ostream& out;
    void operator()(const std::pair< std::string, std::vector< Position > >& p) const;
  };

  struct ContextPrinter
  {
    std::ostream& out;
    const std::vector< std::vector<std::string > >& text;
    size_t radius;
    void operator()(const Position& pos) const;
  };

  struct MergeFunctor
  {
    std::map< std::string, std::vector< Position > >& dest;
    void operator()(const std::pair<std::string, std::vector< Position > >& p) const;
  };

  struct IntersectFunctor
  {
    const std::map< std::string, std::vector< Position > >& reference;
    std::map< std::string, std::vector< Position > >& result;
    void operator()(const std::pair< std::string, std::vector< Position > >& p) const;
  };

  struct DiffFunctor
  {
    const std::map< std::string, std::vector< Position > >& reference;
    std::map< std::string, std::vector< Position > >& result;
    void operator()(const std::pair< std::string, std::vector< Position > >& p) const;
  };

  struct SaveDict
  {
    std::ofstream& file;
    void operator()(const std::pair< std::string, OneXrefDict >& p) const;
  };

  struct SaveWord
  {
    std::ofstream& file;
    void operator()(const std::pair< std::string, std::vector< Position > >& p) const;
  };

  struct SavePosition
  {
    std::ofstream& file;
    mutable bool first = true;
    void operator()(const Position& pos) const;
  };

  struct SaveTextLine
  {
    std::ofstream& file;
    void operator()(const std::vector< std::string >& line) const;
  };

  struct TextRestorer
  {
    std::ofstream& file;
    void operator()(const std::vector< std::string >& line) const;
  };

  struct WordPrinter
  {
    std::ofstream& file;
    mutable bool first = true;
    void operator()(const std::string& word) const;
  };

  struct JoinWords
  {
    std::ostream& out;
    mutable bool first = true;
    void operator()(const std::string& w) const;
  };
}

#endif
