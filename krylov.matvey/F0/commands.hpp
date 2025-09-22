#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iosfwd>
#include <functional>
#include <algorithm>
#include "index.hpp"

namespace krylov
{
  class CommandProcessor
  {
  public:
    CommandProcessor(std::ostream& output);
    void execute(const std::string& line);
  private:
    IndexStorage storage_;
    std::ostream& out_;
    std::map< std::string, IndexDocument > indexes_;
    std::map< std::string, std::function< void(const std::vector< std::string >&) > > commands;
    void initializeCommands();
    void createIndex(const std::string& indexName, const std::string& filename);
    void findWord(const std::string& indexName, const std::string& word);
    void printIndex(const std::string& indexName);
    void deleteIndex(const std::string& indexName);
    void listIndexes();
    void mergeIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void zipTexts(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void intersectIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void weaveTexts(const std::string& index1, const std::string& index2, const std::string& newIndex);
    void diffIndexes(const std::string& index1, const std::string& index2, const std::string& newIndex);

    void intersectIndexesCmd(const std::vector< std::string >& args);
    void weaveTextsCmd(const std::vector< std::string >& args);
    void diffIndexesCmd(const std::vector< std::string >& args);
    void createIndexCmd(const std::vector< std::string >& args);
    void printIndexCmd(const std::vector< std::string >& args);
    void findWordCmd(const std::vector< std::string >& args);
    void mergeIndexesCmd(const std::vector< std::string >& args);
    void zipTextsCmd(const std::vector< std::string >& args);
    void listIndexesCmd(const std::vector< std::string >& args);
    void deleteIndexCmd(const std::vector< std::string >& args);
  };

  struct IsInSecondIndex
  {
    const krylov::IndexDictionary& idx2;
    bool operator()(const krylov::IndexDictionary::value_type& entry) const
    {
      return idx2.find(entry.first) != idx2.end();
    }
  };

  struct MergeEntries
  {
    const krylov::IndexDictionary& idx2;
    krylov::IndexDictionary::value_type operator()(const krylov::IndexDictionary::value_type& entry) const
    {
      std::set< size_t > merged;
      std::set_union(entry.second.begin(), entry.second.end(), idx2.at(entry.first).begin(),
        idx2.at(entry.first).end(), std::inserter(merged, merged.begin()));
      return std::make_pair(entry.first, merged);
    }
  };

  struct IsNotInSecondIndex
  {
    const krylov::IndexDictionary& idx2;
    bool operator()(const krylov::IndexDictionary::value_type& entry) const
    {
      return idx2.find(entry.first) == idx2.end();
    }
  };

  struct WeaveLinesGenerator
  {
    const std::vector< std::string >& lines1;
    const std::vector< std::string >& lines2;
    size_t index1;
    size_t index2;
    bool nextIsFirst;

    WeaveLinesGenerator(const std::vector< std::string >& l1, const std::vector< std::string >& l2):
      lines1(l1), lines2(l2), index1(0), index2(0), nextIsFirst(true)
    {}

    std::string operator()()
    {
      if (nextIsFirst && index1 < lines1.size())
      {
        nextIsFirst = false;
        return lines1[index1++];
      }
      else if (index2 < lines2.size())
      {
        nextIsFirst = true;
        return lines2[index2++];
      }
      else if (index1 < lines1.size())
      {
        return lines1[index1++];
      }
      return "";
    }
  };
}

#endif
