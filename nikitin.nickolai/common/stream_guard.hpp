#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <ios>

namespace nikitin
{
  class StreamGuard
  {
  public:
    explicit StreamGuard(std::basic_ios< char > &);
    ~StreamGuard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
