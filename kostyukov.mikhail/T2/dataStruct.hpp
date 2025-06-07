#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP
#include <iosfwd>
#include <string>

namespace kostyukov
{
  struct DataStruct
  {
    unsigned long long key1 = 0;
    unsigned long long key2 = 0;
    std::string key3 = "";
    bool operator<(const DataStruct& rhs) const;
  };
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  struct BinUllIO
  {
    unsigned long long value;
  };
  struct HexUllIO
  {
    unsigned long long value;
  };
  struct StringIO
  {
    std::string& ref;
  };
  struct ConstStringIO
  {
    const std::string& ref;
  };
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::ostream& operator<<(std::ostream& out, BinUllIO&& dest);
  std::ostream& operator<<(std::ostream& out, HexUllIO&& dest);
  std::ostream& operator<<(std::ostream& out, ConstStringIO&& dest);
}
#endif
