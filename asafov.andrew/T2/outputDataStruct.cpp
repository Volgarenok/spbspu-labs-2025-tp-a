#include "datastruct.h"
#include <iomanip>
#include <bitset>
#include <algorithm>

namespace
{
  void outputULLBin(std::ostream& os, unsigned long long num)
  {
    if (num == 0)
    {
      os << "0b0";
      return;
    }

    os << "0b0";
    std::string bits;
    while (num > 0)
    {
      bits.push_back((num & 1) ? '1' : '0');
      num >>= 1;
    }
    std::reverse(bits.begin(), bits.end());
    os << bits;
  }

  void outputCmpLsp(std::ostream& os, const std::complex< double >& data)
  {
    os << std::fixed << std::setprecision(1);
    os << "#c(" << data.real() << " " << data.imag() << ")";
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
