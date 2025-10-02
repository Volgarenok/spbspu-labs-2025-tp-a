#include "output_struct.hpp"

unsigned mazitov::calcBitWidth(unsigned long long num)
{
  int bits = 0;
  while (num != 0)
  {
    num >>= 1;
    bits++;
  }
  return bits;
}

std::ostream &mazitov::operator<<(std::ostream &out, const UnsignedLongLongBinOutput &dest)
{
  if (dest.ref == 0)
  {
    out << '0';
    return out;
  }

  unsigned bit_len = calcBitWidth(dest.ref);
  if (bit_len == 1)
  {
    out << '0';
  }

  for (int i = bit_len - 1; i >= 0; --i)
  {
    out << ((dest.ref >> i) & 1);
  }
  return out;
}
