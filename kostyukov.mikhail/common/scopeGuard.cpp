#include "scopeGuard.hpp"

kostyukov::ScopeGuard::ScopeGuard(std::basic_ios< char >& s):
  precision_(s.precision()),
  width_(s.width()),
  s_(s),
  fmt_(s.flags()),
  fill_(s.fill())
{}

kostyukov::ScopeGuard::~ScopeGuard()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(fmt_);
  s_.fill(fill_);
}
