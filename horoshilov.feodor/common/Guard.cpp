#include "Guard.h"
horoshilov::Guard::Guard(std::basic_ios< char >& s):
  s_(s),
  fmt_(s.flags()),
  width_(s.width()),
  precision_(s.precision()),
  fill_(s.fill())
{}

horoshilov::Guard::~Guard()
{
  s_.flags(fmt_);
  s_.width(width_);
  s_.precision(precision_);
  s_.fill(fill_);
}

