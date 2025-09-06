#include "stream_guard.hpp"

shak::StreamGuard::StreamGuard(std::basic_ios< char > &s):
  s_(s),
  precision_(s.precision()),
  flags_(s.flags()),
  fill_(s.fill())
{}

shak::StreamGuard::~StreamGuard()
{
  s_.precision(precision_);
  s_.flags(flags_);
  s_.fill(fill_);
}
