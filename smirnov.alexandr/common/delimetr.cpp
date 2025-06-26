#include "delimetr.hpp"

std::istream & smirnov::operator>>(std::istream & in, DelimiterI && dest)
{
  char c = 0;
  in >> c;
  if (!in || c != dest.exp)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
