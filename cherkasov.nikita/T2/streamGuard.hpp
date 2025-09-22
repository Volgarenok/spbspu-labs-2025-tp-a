#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP

#include <iostream>
namespace cherkasov
{
  class StreamGuard
  {
  public:
    StreamGuard(std::basic_ios< char > & s);
    ~StreamGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
