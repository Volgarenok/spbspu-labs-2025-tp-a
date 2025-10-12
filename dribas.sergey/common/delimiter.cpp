#include "delimiter.hpp"
#include "streamGuard.hpp"

namespace dribas
{
  std::istream& operator>>(std::istream& in, DelimiterI&& delimiter)
  {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    StreamGuard guard(in);

    char c = '0';
    in >> c;
    if (in && std::tolower(c) != delimiter.exp) {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}
