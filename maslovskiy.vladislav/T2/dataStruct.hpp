#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <string>
#include <iosfwd>
//#include <delimeterIO.hpp>

namespace maslovskiy
{
  struct DataStruct
  {
    long long key1;
    unsigned long long key2;
    std::string key3;
  };
  std::ostream& operator<<(std::ostream& out, const DataStruct& data);
  std::istream& operator>>(std::istream& in, DataStruct& data);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
  struct SllLitI
  {
    long long& ref;
  };
  std::istream& operator>>(std::istream& in, SllLitI&& dest);
  struct SllLitO
  {
    const long long& ref;
  };
  std::ostream& operator<<(std::ostream& out, const SllLitO&& dest);
  struct UllHexIO
  {
    unsigned long long& ref;
  };
  std::istream& operator>>(std::istream& in, UllHexIO&& dest);
  struct StringIO
  {
    std::string& ref;
  };
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif
