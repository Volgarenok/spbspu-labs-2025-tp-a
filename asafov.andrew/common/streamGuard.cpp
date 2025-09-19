#include "streamGuard.h"

asafov::StreamGuard::StreamGuard(std::ios& s):
stream(s),
flags(s.flags()),
precision(s.precision()),
width(s.width()),
fill(s.fill()) {}

asafov::StreamGuard::~StreamGuard()
{
  stream.flags(flags);
  stream.precision(precision);
  stream.width(width);
  stream.fill(fill);
}
