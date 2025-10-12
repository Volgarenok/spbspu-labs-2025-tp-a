#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>

namespace gavrilova {

  struct DataStruct {
    double key1;
    long long key2;
    std::string key3;
  };

  struct DelimiterIO {
    char exp;
  };

  struct SymbolIO {
    char exp;
  };

  struct DoubleI {
    double& ref;
  };

  struct DoubleO {
    const double& ref;
  };

  struct LongLongIO {
    long long& ref;
  };

  struct StringWithQuotsIO {
    std::string& ref;
  };

  struct StringI_with_spaceIO {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, SymbolIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, LongLongIO&& dest);
  std::istream& operator>>(std::istream& in, StringWithQuotsIO&& dest);
  std::istream& operator>>(std::istream& in, StringI_with_spaceIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleO& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

  bool compare(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
