#ifndef DATA_H
#define DATA_H

#include <array>
#include <iostream>
#include <string>

namespace horoshilov
{
  struct DataStruct
  {
    double key1;
    double key2;
    std::string key3;
  };

  struct KeyIO
  {
    DataStruct& data;
    std::array< bool, 4 > usedKeys = { false };
  };

  struct DoubleSciI
  {
    double& ref;
  };

  struct DoubleI
  {
    double& ref;
  };

  struct StringI
  {
    std::string& ref;
  };

  struct DoubleSciO
  {
    const double& ref;
  };

  struct DoubleO
  {
    const double& ref;
  };

  struct StringO
  {
    const std::string& ref;
  };

  std::istream& operator>>(std::istream& in, DoubleSciI&& dest);
  std::istream& operator>>(std::istream& in, DoubleI&& dest);
  std::istream& operator>>(std::istream& in, StringI&& dest);
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::istream& operator>>(std::istream& input, KeyIO&& dest);
  std::ostream& operator<<(std::ostream& output, const DoubleSciO& dest);
  std::ostream& operator<<(std::ostream& out, const DoubleO& dest);
  std::ostream& operator<<(std::ostream& output, const StringO& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& src);

  bool compareData(const DataStruct& a, const DataStruct& b);
}

#endif
