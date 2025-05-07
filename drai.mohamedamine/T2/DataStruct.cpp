#include "DataStruct.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace nspace {

bool isValidDataStruct(const std::string& line) {
    return line.find("key1") != std::string::npos &&
           line.find("key2") != std::string::npos &&
           line.find("key3") != std::string::npos;
}

std::string extractBetween(const std::string& line, const std::string& key) {
    size_t pos = line.find(key);
    if (pos == std::string::npos) return "";

    size_t valueStart = line.find(' ', pos);
    if (valueStart == std::string::npos) return "";

    size_t valueEnd = line.find(':', valueStart);
    if (valueEnd == std::string::npos) return "";

    return line.substr(valueStart + 1, valueEnd - valueStart - 1);
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    std::getline(in, line);

    if (!isValidDataStruct(line)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        // key1: parse scientific double manually
        std::string key1_str = extractBetween(line, "key1");
        data.key1 = std::stod(key1_str);

        // key2: parse rational
        size_t key2_pos = line.find("key2");
        size_t n_pos = line.find("N ", key2_pos);
        size_t d_pos = line.find("D ", key2_pos);
        size_t n_colon = line.find(":", n_pos);
        size_t d_colon = line.find(":", d_pos);

        std::string n_str = line.substr(n_pos + 2, n_colon - n_pos - 2);
        std::string d_str = line.substr(d_pos + 2, d_colon - d_pos - 2);

        data.key2.first = std::stoll(n_str);
        data.key2.second = std::stoull(d_str);

        // key3: extract string between quotes
        size_t key3_pos = line.find("key3");
        size_t first_quote = line.find('"', key3_pos);
        size_t second_quote = line.find('"', first_quote + 1);
        data.key3 = line.substr(first_quote + 1, second_quote - first_quote - 1);
    } catch (...) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << std::scientific << std::setprecision(1) << data.key1
        << ":key2 (:N " << data.key2.first << ":D " << data.key2.second
        << ":):key3 "" << data.key3 << "":)";
    return out;
}

} // namespace nspace
