#include "IOFmtguard.h"

voronina::iofmtguard::iofmtguard(std::basic_ios< char > &s):
  s_(s),
  width_(s.width()),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}

voronina::iofmtguard::~iofmtguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
