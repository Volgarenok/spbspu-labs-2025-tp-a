#include "scopeGuard.hpp"
detail::ScopeGuard::ScopeGuard(std::basic_ios< char >& s):
  s_(s),
  fmt_(s.flags()),
  width_(s.width()),
  precision_(s.precision()),
  fill_(s.fill())
{}

detail::ScopeGuard::~ScopeGuard()
{
  s_.flags(fmt_);
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
}
