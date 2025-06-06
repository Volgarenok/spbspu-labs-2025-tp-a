#include "io_helpers.hpp"
#include <iomanip>

namespace beshimow {
    std::istream& operator>>(std::istream& in, DelimeterIO&& dest) {
        std::istream::sentry sentry(in);
        if (sentry) {
            char c;
            if (in >> c && c != dest.exp) {
                in.setstate(std::ios::failbit);
            }
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, DelimeterIO&& dest) {
        out << dest.exp;
        return out;
    }

    std::istream& operator>>(std::istream& in, DoubleEIOIn&& dest) {
        return in >> dest.ref;
    }

    std::ostream& operator<<(std::ostream& out, DoubleEIOOut&& dest) {
        return out << dest.ref;
    }

    std::istream& operator>>(std::istream& in, PairLLIOIn&& dest) {
        return in >> dest.ref.first >> dest.ref.second;
    }

    std::ostream& operator<<(std::ostream& out, PairLLIOOut&& dest) {
        return out << dest.ref.first << ' ' << dest.ref.second;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        return std::getline(in >> std::ws, dest.ref);
    }
}
