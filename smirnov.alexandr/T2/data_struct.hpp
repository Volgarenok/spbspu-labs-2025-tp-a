#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <iostream>
#include <string>
#include <delimetr.hpp>

namespace smirnov
{
  struct DataStruct
  {
    unsigned long long key1;
    long long key2;
    std::string key3;
  };
  struct ULLIO
  {
    unsigned long long & ref;
  };
  struct LLIO
  {
    long long & ref;
  };
  struct QuotedStringIO
  {
    std::string & ref;
  };
  std::istream & operator>>(std::istream & in, ULLIO && dest);
  std::istream & operator>>(std::istream & in, LLIO && dest);
  std::istream & operator>>(std::istream & in, QuotedStringIO && dest);
  bool compare(const DataStruct & lhs, const DataStruct & rhs);
  std::istream & operator>>(std::istream & in, DataStruct & data);
  std::ostream & operator<<(std::ostream & out, const DataStruct & data);
}
#endif
