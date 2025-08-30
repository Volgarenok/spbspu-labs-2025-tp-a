#ifndef INPUT_STRUCT_HPP
#define INPUT_STRUCT_HPP

#include "data-struct.hpp"

namespace aleksandrov
{
  struct DelimeterI
  {
    char exp;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct RationalI
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct LongLongI
  {
    long long& ref;
  };

  struct UnsignedLongLongI
  {
    unsigned long long& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream&, DelimeterI&&);
  std::istream& operator>>(std::istream&, DoubleI&&);
  std::istream& operator>>(std::istream&, RationalI&&);
  std::istream& operator>>(std::istream&, LongLongI&&);
  std::istream& operator>>(std::istream&, UnsignedLongLongI&&);
  std::istream& operator>>(std::istream&, DataStruct&);
}

#endif

