#include "streamGuard.hpp"

maslovskiy::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

maslovskiy::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(fmt_);
  s_.fill(fill_);
}
