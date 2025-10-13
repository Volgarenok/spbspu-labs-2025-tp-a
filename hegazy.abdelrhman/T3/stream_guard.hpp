#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <iostream>

namespace bob
{
  class Iofmtguard
  {
  public:
    explicit Iofmtguard(std::basic_ios< char > &stream);
    ~Iofmtguard();

  private:
    std::basic_ios< char > &stream_;
    std::streamsize precision_;
    std::streamsize width_;
    char fill_;
    std::ios::fmtflags flags_;
  };
}
#endif
