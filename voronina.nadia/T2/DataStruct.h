#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

namespace nspace
{
  struct DataStruct
  {
    double key1;
    unsigned long long int key2;
    std::string key3;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleIO
  {
    double &ref;
  };

  struct UnsignedLongLongIO
  {
    unsigned long long int &ref;
  };

  struct StringIO
  {
    std::string &ref;
  };

  class StreamGuard
  {
  public:
    StreamGuard(std::basic_ios<char> &s);
    ~StreamGuard();

  private:
    std::basic_ios<char> &s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, DoubleIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, UnsignedLongLongIO &&dest);
  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &dest);

  bool comparator(DataStruct &left, DataStruct &right);
}
#endif
