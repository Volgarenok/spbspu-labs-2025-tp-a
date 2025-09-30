#include "structs.hpp"
#include "stream_guard.hpp"
#include <cmath>
#include <iostream>

namespace kalmbah {

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    in >> Delimiter{'('};
    while (true) {
        std::string key;
        in >> Delimiter{':'} >> key;

        if (key == "key1") {
            in >> UllOct{temp.key1_};
            hasKey1 = true;
        }
        else if (key == "key2") {
            in >> RatLsp{temp.key2_};
            hasKey2 = true;
        }
        else if (key == "key3") {
            in >> StringToken{temp.key3_};
            hasKey3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
            break;
        }

        if (hasKey1 && hasKey2 && hasKey3) break;
    }

    in >> Delimiter{':'} >> Delimiter{')'};

    if (in && hasKey1 && hasKey2 && hasKey3) {
        data = std::move(temp);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;

    kalmbah::StreamGuard guard(out);
    out << "(:key1 0" << std::oct << data.key1_ << std::dec;
    out << ":key2 (:N " << data.key2_.first
        << ":D " << data.key2_.second << ":)";
    out << ":key3 \"" << data.key3_ << "\":)";
    return out;
}

bool operator<(const DataStruct& left, const DataStruct& right) {
    if (left.key1_ != right.key1_) {
        return left.key1_ < right.key1_;
    }

    const double leftVal = static_cast<double>(left.key2_.first) / left.key2_.second;
    const double rightVal = static_cast<double>(right.key2_.first) / right.key2_.second;
    constexpr double epsilon = 1e-9;

    if (std::abs(leftVal - rightVal) > epsilon) {
        return leftVal < rightVal;
    }

    return left.key3_.size() < right.key3_.size();
}

}
