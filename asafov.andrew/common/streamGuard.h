#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace asafov
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::ios& s);
    ~StreamGuard();

  private:
    std::ios& stream;
    std::ios::fmtflags flags;
    std::streamsize precision;
    std::streamsize width;
    char fill;
  };
}

#endif
