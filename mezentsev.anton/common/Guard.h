#ifndef GUARD_H
#define GUARD_H
#include <ios>

namespace mezentsev
{
  class Guard
  {
  public:
    Guard(const Guard&) = delete;
    Guard(Guard&&) = delete;
    explicit Guard(std::basic_ios< char >& s);
    ~Guard();
  private:
    std::basic_ios< char >& s_;
    std::basic_ios< char >::fmtflags fmt_;
    std::streamsize width_;
    std::streamsize precision_;
    char fill_;
  };
}
#endif
