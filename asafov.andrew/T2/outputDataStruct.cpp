#include "datastruct.h"
#include <iterator>
#include <algorithm>
#include <iomanip>

namespace
{
  std::string cmpLspToStr(const std::complex<double> data)
  {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << data.real();
    std::string re = oss.str();
    oss.str("");
    oss << data.imag();
    std::string im = oss.str();
    return "#c(" + re + " " + im + ")";
  }

  std::string ullBinToStr(unsigned long long num)
  {
    if (num == 0)
    {
      return "0b0";
    }
    std::string binary;
    for (int i = sizeof(num) * 8 - 1; i >= 0; --i)
    {
      binary += (num & (1ULL << i)) ? '1' : '0';
    }
    size_t firstOne = binary.find('1');
    if (firstOne != std::string::npos)
    {
      binary = binary.substr(firstOne);
    }
    return "0b0" + binary;
  }
}

std::ostream& asafov::operator<<(std::ostream& os, const DataStruct& data)
{
  os << "(:key1 " << ullBinToStr(data.key1) << ":key2 " << cmpLspToStr(data.key2)<< ":key3 \"" << data.key3 << "\":)";
  return os;
}
