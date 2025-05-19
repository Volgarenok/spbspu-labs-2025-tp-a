#include "data_struct.hpp"
#include "io_helpers.hpp"
#include "stream_guard.hpp"

namespace beshimow {
    std::istream& operator>>(std::istream& in, DataStruct& dst) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct temp;
        bool hasKeys[3] = {false};

        in >> DelimeterIO{'('};

        for (int i = 0; i < 3; ++i) {
            in >> DelimeterIO{':'};
            in >> DelimeterIO{'k'};
            in >> DelimeterIO{'e'};
            in >> DelimeterIO{'y'};

            int keyNum = 0;
            in >> keyNum;

            if (keyNum == 1 && !hasKeys[0]) {
                in >> DoubleEIOIn{temp.key1};
                hasKeys[0] = true;
            }
            else if (keyNum == 2 && !hasKeys[1]) {
                in >> PairLLIOIn{temp.key2};
                hasKeys[1] = true;
            }
            else if (keyNum == 3 && !hasKeys[2]) {
                in >> StringIO{temp.key3};
                hasKeys[2] = true;
            }
            else {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        in >> DelimeterIO{':'} >> DelimeterIO{')'};

        if (in && hasKeys[0] && hasKeys[1] && hasKeys[2]) {
            dst = temp;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& dst) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;

        StreamGuard guard(out);
        out << "(:key1 " << DoubleEIOOut{dst.key1}
            << ":key2 " << PairLLIOOut{dst.key2}
            << ":key3 \"" << dst.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& lhs, const DataStruct& rhs) {
        if (lhs.key1 != rhs.key1) return lhs.key1 < rhs.key1;
        if (lhs.key2 != rhs.key2) return lhs.key2 < rhs.key2;
        return lhs.key3.size() < rhs.key3.size();
    }
}
