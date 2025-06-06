#include "format_guard.hpp"

filonova::FormatGuard::FormatGuard(std::basic_ios< char >& s):
  s_(s),
  fmt_(s.flags()),
  precision_(s.precision()),
  fill_(s.fill())
{}

filonova::FormatGuard::~FormatGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}

