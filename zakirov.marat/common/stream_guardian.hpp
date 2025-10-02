#ifndef STREAM_GUARDIAN_HPP
#define STREAM_GUARDIAN_HPP
#include <iostream>

namespace zakirov
{
  class Guardian
  {
  public:
    explicit Guardian(std::basic_ios< char > & s);
    ~Guardian();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}

#endif
