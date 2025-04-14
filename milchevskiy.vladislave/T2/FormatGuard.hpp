#ifndef FORMATGUARD_HPP
#define FORMATGUARD_HPP

#include <ios>

namespace milchevskiy
{
    class FormatGuard
    {
    public:
        FormatGuard(std::basic_ios< char >& s);
        ~FormatGuard();

    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };
}
#endif;