#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <iterator>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>

namespace kurbyko
{
  struct DataStruct
  {
    unsigned long long key1;
    char key2;
    std::string key3;
    bool operator<(const DataStruct& other) const;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct ULLBinary
  {
    unsigned long long& value;
  };

  struct CharI
  {
    char& value;
  };

  struct StrKeyI
  {
    std::string& value;
  };

  struct StringIO
  {
    const char* expected;
  };

  class iofmtguard
  {
  public:
    iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::string ullToBinaryString(unsigned long long value);
  unsigned long long binaryStringToULL(const std::string& str);

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, ULLBinary&& key);
  std::istream& operator>>(std::istream& in, CharI&& key);
  std::istream& operator>>(std::istream& in, StrKeyI&& key);
  std::istream& operator>>(std::istream& in, const StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
