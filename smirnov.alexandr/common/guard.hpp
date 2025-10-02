#ifndef GUARD_HPP
#define GUARD_HPP
#include <iostream>

namespace smirnov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > & stream);
    StreamGuard(const StreamGuard &) = delete;
    StreamGuard & operator=(const StreamGuard &) = delete;
    ~StreamGuard();
  private:
    std::basic_ios< char > & stream_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}
#endif
