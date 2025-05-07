#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <string>
#include <vector>

namespace horoshilov
{
  struct DataStruct
  {
    double key1 = 0;
    double key2 = 0;
    std::string key3 = "";
  };

  struct KeyIO
  {
    DataStruct& data;
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct DoubleSciIO
  {
    double& ref;
  };

  struct DoubleIO
  {
    double& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleSciIO&& dest);
  std::istream& operator>>(std::istream& in, DoubleIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& input, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& output, const DoubleSciIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleIO&& dest);
  std::ostream& operator<<(std::ostream& output, const StringIO&& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  bool compareData(const DataStruct& a, const DataStruct& b);
}

#endif
