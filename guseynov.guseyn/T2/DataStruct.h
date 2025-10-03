#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <complex>
#include <ios>

namespace guseynov
{
  struct DataStruct
  {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
  };

  struct FormatGuard
  {
    explicit FormatGuard(std::basic_ios<char>& s);
    FormatGuard(const FormatGuard&) = delete;
    FormatGuard& operator=(const FormatGuard&) = delete;
    ~FormatGuard();

  private:
    std::basic_ios<char>& s_;
    std::basic_ios<char>::fmtflags fmt_;
    std::streamsize precision_;
    char fill_;
  };

  struct DelimiterValue
  {
    char exp;
  };
  struct HexValue
  {
    unsigned long long& ref;
  };
  struct ComplexValue
  {
    std::complex<double>& ref;
  };
  struct StringValue
  {
    std::string& ref;
  };

  bool operator<(const DataStruct& lhs, const DataStruct& rhs);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);
  std::istream& operator>>(std::istream& in, DelimiterValue&& dest);
  std::istream& operator>>(std::istream& in, HexValue&& dest);
  std::istream& operator>>(std::istream& in, ComplexValue&& dest);
  std::istream& operator>>(std::istream& in, StringValue&& dest);
}

#endif
