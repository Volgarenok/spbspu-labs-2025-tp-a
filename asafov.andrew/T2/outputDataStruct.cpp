#include "datastruct.h"
#include <iomanip>
#include <algorithm>
#include <streamGuard.h>

namespace
{
  void outputULLBin(std::ostream& os, unsigned long long num)
  {
    os << "0b0";

    if (num == 0)
    {
      return;
    }

    int highest = 63;
    while (((num >> highest) & 1ULL) == 0)
    {
      --highest;
    }

    for (int i = highest; i >= 0; --i)
    {
      os << (((num >> i) & 1ULL) ? '1' : '0');
    }
  }

  void outputCmpLsp(std::ostream& os, const std::complex< double >& data)
  {
    os << std::fixed << std::setprecision(1);
    os << "#c(" << data.real() << " " << data.imag() << ")";
  }
}

std::ostream& asafov::operator<<(std::ostream& os, const DataStruct& data)
{
  StreamGuard guard(os);
  os << "(:key1 ";
  outputULLBin(os, data.key1);
  os << ":key2 ";
  outputCmpLsp(os, data.key2);
  os << ":key3 \"" << data.key3 << "\":)";
  return os;
}
