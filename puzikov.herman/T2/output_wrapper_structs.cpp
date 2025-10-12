#include "output_wrapper_structs.hpp"
#include <cmath>
#include <format_guard.hpp>

namespace
{
  unsigned calcBitWidth(unsigned long long num)
  {
    if (num == 0)
    {
      return 1;
    }
    return static_cast< unsigned >(std::log2(num)) + 1;
  }

  std::ostream &outputBinRepresentation(unsigned long long num, std::ostream &os)
  {
    int bitWidth = calcBitWidth(num);
    os << '0';
    os << 'b';

    for (int i = bitWidth - 1; i >= 0; --i)
    {
      os << ((num >> i) & 1);
    }
    return os;
  }
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const ULLValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);

  if (source.ref == 1)
  {
    return out << "0b01";
  }

  return outputBinRepresentation(source.ref, out);
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const PairValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << "(:N " << source.ref.first << ":D " << source.ref.second << ":)";
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const StringValue &source)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << '\"' << source.ref << '\"';
}
