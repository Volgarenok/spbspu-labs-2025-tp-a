#include "datastruct.h"
#include <iterator>
#include <algorithm>
#include <iomanip>

namespace
{
  void outputCmpLsp(std::ostream& os, const std::complex<double>& data)
  {
    os << std::fixed << std::setprecision(1);
    os << "#c(" << data.real() << " " << data.imag() << ")";
  }

  void outputULLBin(std::ostream& os, unsigned long long num)
  {
    if (num == 0)
    {
      os << "0b0";
      return;
    }
    
    os << "0b0";
    bool leadingZero = true;
    for (int i = sizeof(num) * 8 - 1; i >= 0; --i)
    {
      bool bit = num & (1ULL << i);
      if (!leadingZero || bit)
      {
        os << (bit ? '1' : '0');
        leadingZero = false;
      }
    }
    if (leadingZero)
    {
      os << '0';
    }
  }
}

std::ostream& asafov::operator<<(std::ostream& os, const DataStruct& data)
{
  os << "(:key1 ";
  outputULLBin(os, data.key1);
  os << ":key2 ";
  outputCmpLsp(os, data.key2);
  os << ":key3 \"" << data.key3 << "\":)";
  return os;
}