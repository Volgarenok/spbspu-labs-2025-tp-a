#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

#include <iosfwd>
#include "dataStruct.hpp"

namespace maslov
{
  struct DoubleSciIO
  {
    double & ref;
  };
  struct CharLitIO
  {
    char & ref;
  };
  struct StringIO
  {
    std::string & ref;
  };
  std::istream & operator>>(std::istream & in, DoubleSciIO && dest);
  std::istream & operator>>(std::istream & in, CharLitIO && dest);
  std::istream & operator>>(std::istream & in, StringIO && dest);
  std::istream & operator>>(std::istream & in, DataStruct & data);
  std::ostream & operator<<(std::ostream & out, const DoubleSciIO & dest);
  std::ostream & operator<<(std::ostream & out, const DataStruct & data);
}

#endif
