#ifndef STREAMGUARD_HPP
#define STREAMGUARD_HPP

#include <iomanip>

namespace klimova
{
  class Streamguard
  {
  public:
    explicit Streamguard(std::basic_ios< char >& s);
    ~Streamguard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };
}
#endif
