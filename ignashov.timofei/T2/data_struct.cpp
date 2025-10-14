#include "data_struct.hpp"
#include <cctype>
#include <cmath>

namespace ignashov {

    std::istream& operator>>(std::istream& in, DataStruct& data)
    {
        DataStruct temp;
        bool key1Found = false;
        bool key2Found = false;
        bool key3Found = false;

        std::string line;
        if (!std::getline(in, line)) {
            return in;
        }

        if (line.empty() || line.front() != '(' || line.back() != ')') {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string content = line.substr(1, line.length() - 2);

        size_t pos = 0;
        while (pos < content.length()) {
            while (pos < content.length() && std::isspace(content[pos])) {
                ++pos;
            }
            if (pos >= content.length()) {
                break;
            }

            if (content[pos] != ':') {
                in.setstate(std::ios::failbit);
                return in;
            }
            ++pos;

            std::string keyName;
            while (pos < content.length() && !std::isspace(content[pos]) && content[pos] != ':') {
                keyName += content[pos];
                ++pos;
            }

            while (pos < content.length() && std::isspace(content[pos])) {
                ++pos;
            }

            if (keyName == "key1") {
                if (pos + 2 >= content.length() ||
                    (content[pos] != '0' && content[pos + 1] != 'b' && content[pos + 1] != 'B')) {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                pos += 2;
                std::string binaryStr;
                while (pos < content.length() && (content[pos] == '0' || content[pos] == '1')) {
                    binaryStr += content[pos];
                    ++pos;
                }

                if (binaryStr.empty()) {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                temp.key1 = 0;
                for (char c : binaryStr) {
                    temp.key1 = (temp.key1 << 1) | (c - '0');
                }

                key1Found = true;
            }
            else if (keyName == "key2") {
                if (pos + 3 >= content.length() ||
                    content[pos] != '#' || content[pos + 1] != 'c' || content[pos + 2] != '(') {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                pos += 3;

                std::string realStr;
                while (pos < content.length() && !std::isspace(content[pos]) && content[pos] != ')') {
                    realStr += content[pos];
                    ++pos;
                }

                while (pos < content.length() && std::isspace(content[pos])) {
                    ++pos;
                }

                std::string imagStr;
                while (pos < content.length() && !std::isspace(content[pos]) && content[pos] != ')') {
                    imagStr += content[pos];
                    ++pos;
                }

                if (pos >= content.length() || content[pos] != ')') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                ++pos;

                try {
                    double real = std::stod(realStr);
                    double imag = std::stod(imagStr);
                    temp.key2 = std::complex<double>(real, imag);
                    key2Found = true;
                }
                catch (...) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
            }
            else if (keyName == "key3") {
                if (pos >= content.length() || content[pos] != '"') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                ++pos;

                std::string strValue;
                while (pos < content.length() && content[pos] != '"') {
                    strValue += content[pos];
                    ++pos;
                }

                if (pos >= content.length() || content[pos] != '"') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                ++pos;

                temp.key3 = strValue;
                key3Found = true;
            }

            while (pos < content.length() && content[pos] != ':') {
                if (!std::isspace(content[pos])) {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                ++pos;
            }
        }

        if (!key1Found || !key2Found || !key3Found) {
            in.setstate(std::ios::failbit);
            return in;
        }

        data = temp;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data)
    {
        out << "(:key1 0b";

        if (data.key1 == 0) {
            out << "0";
        }
        else {
            std::string binaryStr;
            unsigned long long n = data.key1;
            while (n > 0) {
                binaryStr = (n & 1 ? "1" : "0") + binaryStr;
                n >>= 1;
            }
            out << binaryStr;
        }

        out << ":key2 #c(" << data.key2.real() << " " << data.key2.imag()
            << "):key3 \"" << data.key3 << "\":)";

        return out;
    }

    bool operator<(const DataStruct& lhs, const DataStruct& rhs)
    {
        if (lhs.key1 != rhs.key1) {
            return lhs.key1 < rhs.key1;
        }
        if (std::abs(lhs.key2.real() - rhs.key2.real()) > 1e-10 ||
            std::abs(lhs.key2.imag() - rhs.key2.imag()) > 1e-10) {
            return std::abs(lhs.key2) < std::abs(rhs.key2);
        }
        return lhs.key3.length() < rhs.key3.length();
    }

} // namespace ignashov