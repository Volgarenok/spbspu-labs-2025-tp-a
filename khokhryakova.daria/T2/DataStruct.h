#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <utility>

namespace khokhryakova
{
  class Iofmtguard
  {
  public:
    explicit Iofmtguard(std::ostream& s);
    ~Iofmtguard();
  private:
    std::ostream& s_;
    std::ios_base::fmtflags flags_;
  };

  struct DataStruct
  {
    long long key1;
    std::pair< long long, unsigned long long > key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct LongLongIO
  {
    long long& ref;
  };

  struct RationalIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, LongLongIO&& dest);
  std::istream& operator>>(std::istream& in, RationalIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  bool operator<(const DataStruct& a, const DataStruct& b);
}
#endif
