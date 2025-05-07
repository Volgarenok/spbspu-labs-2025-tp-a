#include "data_struct.hpp"

namespace smirnov
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
            in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        in >> dest.ref;
        if (in.get() != 'd')
            in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string data;
        if ((in >> StringIO{ data }) && (data != dest.exp))
            in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, Data& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        Data input;
        {
            using sep = DelimiterIO;
            using label = LabelIO;
            using dbl = DoubleIO;
            using str = StringIO;
            in >> sep{ '(' };
            in >> label{ "key1" } >> sep{ ' ' } >> dbl{ input.key1 } >> sep{ ':' };
            in >> label{ "key2" } >> sep{ ' ' } >> dbl{ input.key2 } >> sep{ ':' };
            in >> label{ "key3" } >> sep{ ' ' } >> str{ input.key3 } >> sep{ ')' };
        }
        if (in)
            dest = input;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const Data& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard fmtguard(out);
        out << std::fixed << std::setprecision(1);
        out << "(:key1 " << src.key1 << "d:";
        out << "key2 " << src.key2 << "d:";
        out << "key3 \"" << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {}

    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}
