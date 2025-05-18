#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <complex>
#include <utility>
#include <string>

struct data_struct_t {
  std::complex<double> key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

std::istream& operator>>(std::istream& in, data_struct_t& data);
std::ostream& operator<<(std::ostream& out, const data_struct_t& data);

#endif
