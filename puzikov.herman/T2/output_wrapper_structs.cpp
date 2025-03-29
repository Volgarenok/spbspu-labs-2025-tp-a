#include "output_wrapper_structs.hpp"
#include "format_guard.hpp"
#include <bitset>
#include <cmath>

std::ostream &puzikov::output::operator<<(std::ostream &out, const ULLValue &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  auto calcBitWidth = [](const unsigned long long &num)
  {
    return 1 + std::log2(num);
  };

  if (dest.ref == 0)
  {
    return out << "0b0";
  }
  else if (dest.ref == 1)
  {
    return out << "0b01";
  }
  return out << "0b" + std::bitset< 64 >(dest.ref).to_string().substr(64 - calcBitWidth(dest.ref));
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const PairValue &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << "(:N " << dest.ref.first << ":D " << dest.ref.second << ":)";
}

std::ostream &puzikov::output::operator<<(std::ostream &out, const StringValue &dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  FormatGuard guard(out);
  return out << '\"' << dest.ref << '\"';
}

