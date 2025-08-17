#include "io-helpers.hpp"
#include "delimiter.hpp"
#include "stream-guard.hpp"
#include <iomanip>
#include <sstream>

std::istream& elagin::operator>>(std::istream& in, const HexIOIn&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string hexStr;
    in >> hexStr;

    if (hexStr.substr(0, 2) != "0x" && hexStr.substr(0, 2) != "0X") {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::istringstream iss(hexStr);
    iss >> std::hex >> dest.ref;

    if (!iss) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& elagin::operator>>(std::istream& in, const ComplexIOIn&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    double real = 0.0;
    double imag = 0.0;

    in >> DelimiterIO{'#'} >> DelimiterIO{'c'}
    >> DelimiterIO{'('} >> real >> imag >> DelimiterIO{')'};

    if (in) {
        dest.ref = std::complex<double>(real, imag);
    }

    return in;
}

std::istream& elagin::operator>>(std::istream& in, const StringIO& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::ostream& elagin::operator<<(std::ostream& out, const HexIOOut& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    StreamGuard guard(out);
    out << "0x" << std::uppercase << std::hex << dest.val;
    return out;
}

std::ostream& elagin::operator<<(std::ostream& out, const ComplexIOOut& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }

    StreamGuard guard(out);
    out << "#c(" << dest.val.real()
        << " " << dest.val.imag() << ")";
    return out;
}