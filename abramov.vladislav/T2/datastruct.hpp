#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP
#include <string>
#include <delimiterIO.hpp>
namespace abramov
{
  struct DataStruct
  {
    double key1;
    unsigned long long int key2;
    std::string key3;
  };
  std::ostream &operator<<(std::ostream &out, const DataStruct &data);
  std::istream &operator>>(std::istream &in, DataStruct &data);
  bool operator<(const DataStruct &lhs, const DataStruct &rhs);

  struct DoubleIO
  {
    double &ref;
  };
  std::istream &operator>>(std::istream &in, DoubleIO &&dest);

  struct UllIO
  {
    unsigned long long int &ref;
  };
  std::istream &operator>>(std::istream &in, UllIO &&dest);

  struct StringIO
  {
    std::string &ref;
  };
  std::istream &operator>>(std::istream &in, StringIO &&dest);
}
#endif
