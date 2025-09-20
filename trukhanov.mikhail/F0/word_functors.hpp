#ifndef WORD_FUNCTORS_HPP
#define WORD_FUNCTORS_HPP

#include <string>
#include <vector>
#include <map>
#include "index.hpp"

namespace trukhanov
{
  struct FindWord
  {
    const std::string& word;
    const ConcordanceIndex& index;
    const std::string& indexName;
    std::ostream& out;
    FindWord(const ConcordanceIndex& index_, const std::string& word_, const std::string& indexName_, std::ostream& out_);
    void operator()() const;
  };

  struct ReplaceWordFunctor
  {
    ConcordanceIndex& index;
    std::string oldWord;
    std::string newWord;
    void operator()();
  };

  struct ExportWordFunctor
  {
    ExportWordFunctor(const std::vector< std::string >& lines, std::ostream& out, const std::string& word);
    void operator()(const IndexMap& index) const;
  private:
    const std::vector< std::string >& lines_;
    std::ostream& out_;
    const std::string& word_;
  };

  bool lengthGreater(const std::string& a, const std::string& b);

  bool lengthLess(const std::string& a, const std::string& b);

  std::string extractWord(const std::pair< const std::string, std::set< std::size_t > >& pair);

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

  bool compareByFrequencyDesc(const std::pair< std::string, std::size_t >& a, const std::pair< std::string, std::size_t >& b);

  std::pair< std::string, std::size_t > frequencyCollector(const IndexMap::value_type& entry);
}

#endif
