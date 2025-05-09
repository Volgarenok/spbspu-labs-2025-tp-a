#ifndef GUARD_H
#define GUARD_H

#include <iostream>

namespace kharlamov
{
  class Guard
  {
  public:
  Guard(std::basic_ios< char >& s);
  ~Guard();
  private:
  std::basic_ios< char >& s_;
  std::streamsize width_;
  char fill_;
  std::streamsize precision_;
  std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
