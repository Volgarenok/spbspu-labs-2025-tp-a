#include "DataStruct.h"
#include <regex>
#include <iomanip>

bool isValidDataStruct(const std::string& line) {
    return line.find("key1") != std::string::npos &&
           line.find("key2") != std::string::npos &&
           line.find("key3") != std::string::npos;
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    std::getline(in, line);

    if (!isValidDataStruct(line)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        std::smatch match;

        std::regex key1_regex(R"(key1\s+([-+]?[0-9]*\.?[0-9]+[eE][-+]?[0-9]+))");
        std::regex key2_regex(R"(key2\s+\(:N\s+(-?\d+):D\s+(\d+):\))");
        std::regex key3_regex(R"(key3\s+"([^"]*)")");

        if (std::regex_search(line, match, key1_regex)) {
            data.key1 = std::stod(match[1]);
        } else {
            throw std::invalid_argument("Invalid key1");
        }

        if (std::regex_search(line, match, key2_regex)) {
            data.key2 = { std::stoll(match[1]), std::stoull(match[2]) };
        } else {
            throw std::invalid_argument("Invalid key2");
        }

        if (std::regex_search(line, match, key3_regex)) {
            data.key3 = match[1];
        } else {
            throw std::invalid_argument("Invalid key3");
        }
    } catch (...) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << std::scientific << std::setprecision(1) << data.key1
        << ":key2 (:N " << data.key2.first << ":D " << data.key2.second
        << ":):key3 \"" << data.key3 << "\":)";
    return out;
}
