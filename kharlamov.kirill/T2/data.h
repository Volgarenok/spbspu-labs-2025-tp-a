#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>

namespace kharlamov
{
  struct DataStruct
  {
    long long key1;
    unsigned long long key2;
    std::string key3;
  };

  bool compare(const DataStruct& lhs, const DataStruct& rhs);

  struct SymbolIO
  {
    char exp;
  };

  struct LongLongIO
  {
    long long& ref;
  };

  struct UnsignedLongLongIO
  {
    unsigned long long& ref;
  };
  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, SymbolIO&& dest);
  std::istream& operator>>(std::istream& in, LongLongIO&& dest);
  std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}
#endif
