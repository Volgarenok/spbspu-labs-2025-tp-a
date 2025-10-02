#include "format_guard.hpp"

puzikov::FormatGuard::FormatGuard(std::basic_ios< char > &s):
  s_(s),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

puzikov::FormatGuard::~FormatGuard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
