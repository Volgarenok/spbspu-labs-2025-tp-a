#include "stream_guard.hpp"

beshimow::StreamGuard::StreamGuard(std::basic_ios<char>& s)
  : s_(s),
    width_(s.width()),
    precision_(s.precision()),
    flags_(s.flags()),
    fill_(s.fill())
{}

beshimow::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
  s_.width(width_);
  s_.fill(fill_);
}
