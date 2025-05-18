#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <complex>
#include <utility>
#include <string>

struct DataStruct
{
  std::complex<double> key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);

#endif
