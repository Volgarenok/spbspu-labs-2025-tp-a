#ifndef FILTER_FUNCTORS_HPP
#define FILTER_FUNCTORS_HPP

#include <set>
#include "index.hpp"

namespace trukhanov
{
  struct InRange
  {
    std::size_t from;
    std::size_t to;
    bool operator()(std::size_t val) const;
  };

  struct AdjustOffset
  {
    std::size_t offset;
    std::size_t operator()(std::size_t val) const;
  };

  struct FilterAdjustPair
  {
    std::size_t from;
    std::size_t to;
    std::pair< std::string, std::set< std::size_t > > operator()(
      const std::pair< const std::string,
      std::set< std::size_t > >& pair) const;
  };

  struct LineFilter
  {
    std::size_t from;
    std::size_t to;
    const ConcordanceIndex& src;
    ConcordanceIndex& dest;
    LineFilter(const ConcordanceIndex& s, ConcordanceIndex& d, std::size_t f, std::size_t t);
    void operator()() const;
  };
}

#endif
