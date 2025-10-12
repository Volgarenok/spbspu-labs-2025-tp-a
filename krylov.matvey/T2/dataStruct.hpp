#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <delimeterIO.hpp>

namespace krylov
{
  struct DataStruct
  {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
  };

  std::ostream& operator<<(std::ostream& out, const DataStruct& data);
  std::istream& operator>>(std::istream& in, DataStruct& data);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);

  struct UllHexIO
  {
    unsigned long long int& ref;
  };
  std::istream& operator>>(std::istream& in, UllHexIO&& dest);

  struct UllBinI
  {
    unsigned long long int& ref;
  };
  std::istream& operator>>(std::istream& in, UllBinI&& dest);

  struct UllBinO
  {
    const unsigned long long int& ref;
  };
  std::ostream& operator<<(std::ostream& out, const UllBinO&& dest);

  struct StringIO
  {
    std::string& ref;
  };
  std::istream& operator>>(std::istream& in, StringIO&& dest);
}

#endif

