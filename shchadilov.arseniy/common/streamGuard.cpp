#include "streamGuard.hpp"

shchadilov::StreamGuard::StreamGuard(std::basic_ios< char >& s):
  precision_(s.precision()),
  width_(s.width()),
  s_(s),
  fmt_(s.flags()),
  fill_(s.fill())
{}

shchadilov::StreamGuard::~StreamGuard()
{
  s_.flags(fmt_);
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
}
