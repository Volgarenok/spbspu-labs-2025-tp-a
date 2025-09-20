#ifndef OUTPUT_FUNCTORS_HPP
#define OUTPUT_FUNCTORS_HPP

#include <string>
#include <iosfwd>
#include <map>
#include <set>
#include <vector>
#include "index.hpp"

namespace trukhanov
{
  struct MergeLinesFunctor
  {
    const std::vector< std::string >& lines1;
    const std::vector< std::string >& lines2;
    std::size_t i;
    std::string operator()();
  };

  std::string formatIndexEntry(const std::pair< const std::string, ConcordanceIndex >& pair);

  struct AddOffset
  {
    std::size_t offset;
    std::size_t operator()(std::size_t val) const;
  };

  struct MergeWithOffset
  {
    ConcordanceIndex& dest;
    std::size_t offset;
    void operator()(const std::pair<const std::string, std::set< std::size_t > >& pair) const;
  };

  WordEntry pairToWordEntry(const IndexMap::value_type& p);

  std::string showEntryToString(const std::pair< std::string, std::size_t >& p);
}

#endif
