#ifndef FMTGUARD_HPP
#define FMTGUARD_HPP

#include <ios>

namespace rychkov
{
  template< class CharT, class Traits = std::char_traits< CharT > >
  class basic_fmtguard
  {
  public:
    using char_type = CharT;
    using traits_type = Traits;
    basic_fmtguard(std::basic_ios< CharT, Traits >& in):
      ios_(in),
      flags_(in.flags()),
      width_(in.width()),
      precision_(in.precision()),
      fill_(in.fill())
    {}
    basic_fmtguard(const basic_fmtguard&) = delete;
    ~basic_fmtguard()
    {
      restore();
    }
    void restore()
    {
      apply(ios_);
    }
    void apply(std::basic_ios< CharT, Traits >& dest)
    {
      dest.flags(flags_);
      dest.width(width_);
      dest.precision(precision_);
      dest.fill(fill_);
    }
  private:
    std::basic_ios< CharT, Traits >& ios_;
    std::ios::fmtflags flags_;
    std::streamsize width_;
    std::streamsize precision_;
    char_type fill_;
  };
  using fmtguard = basic_fmtguard< char >;
  using wfmtguard = basic_fmtguard< wchar_t >;
}

#endif
