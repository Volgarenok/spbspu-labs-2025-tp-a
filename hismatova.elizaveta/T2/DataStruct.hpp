#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <complex>
#include <string>
#include <iterator>
#include <algorithm>
#include <cmath>

struct DataStruct
{
  unsigned long long key1 = 0;
  std::complex< double > key2 = {0, 0};
  std::string key3 = "";
  bool empty() const
  {
    return key1 == 0 && key2.real() == 0 && key2.imag() == 0 && key3.empty();
  }
};
std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
