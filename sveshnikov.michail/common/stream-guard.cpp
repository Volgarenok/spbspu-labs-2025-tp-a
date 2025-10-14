#include "stream-guard.hpp"

sveshnikov::StreamGuard::StreamGuard(std::basic_ios< char > &s):
  s_(s),
  fill_(s.fill()),
  width_(s.width()),
  precision_(s.precision()),
  flags_(s.flags())
{}

sveshnikov::StreamGuard::~StreamGuard()
{
  s_.fill(fill_);
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(flags_);
}
