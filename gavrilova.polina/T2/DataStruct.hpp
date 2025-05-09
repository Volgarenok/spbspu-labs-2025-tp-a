#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iostream>
#include <sstream>

namespace gavrilova
{

  struct DataStruct
  {
    double key1;
    long long key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct SymbolIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct LongLongIO
  {
    long long& ref;
  };

  struct String_with_quotsIO
  {
    std::string& ref;
  };

  struct StringI_with_spaceIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, SymbolIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, LongLongIO&& dest);
  std::istream& operator>>(std::istream& in, String_with_quotsIO&& dest);
  std::istream& operator>>(std::istream& in, StringI_with_spaceIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compare(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
