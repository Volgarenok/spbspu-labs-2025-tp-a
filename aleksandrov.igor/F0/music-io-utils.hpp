#ifndef MUSIC_IO_UTILS_HPP
#define MUSIC_IO_UTILS_HPP

#include <ios>
#include "music.hpp"

namespace aleksandrov
{
  std::istream& operator>>(std::istream&, Note&);
  std::istream& operator>>(std::istream&, Interval&);
  std::istream& operator>>(std::istream&, Chord&);
  std::istream& operator>>(std::istream&, MusicalElement&);

  std::ostream& operator<<(std::ostream&, const Note&);
  std::ostream& operator<<(std::ostream&, const Interval&);
  std::ostream& operator<<(std::ostream&, const Chord&);
  std::ostream& operator<<(std::ostream&, const MusicalElement&);

  template< class Key, class Value >
  struct KeyReturner
  {
    const Key& operator()(const std::pair< Key, Value >& pair) const
    {
      return pair.first;
    }
  };
}

#endif

