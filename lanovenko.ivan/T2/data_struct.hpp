#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <iostream>
#include <delimiter.hpp>
#include <scope_guard.hpp>

namespace lanovenko
{
  struct DataStruct
  {
    char key1;
    std::pair < long long, unsigned long long > key2;
    std::string key3;
  };

  struct CharIO
  {
    char& ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, CharIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool compare(const DataStruct& lhs, const DataStruct& rhs);
}

#endif
