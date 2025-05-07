#include "scope_guard.hpp"

lanovenko::iofmtguard::iofmtguard(std::basic_ios< char >& s):
  s_(s),
  fill_(s.fill()),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

lanovenko::iofmtguard::~iofmtguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
