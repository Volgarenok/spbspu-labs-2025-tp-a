#define _CRT_SECURE_NO_WARNINGS
#include "data_struct.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

namespace smirnov {
    namespace {
        bool parseDoubleLiteral(const std::string& s, double& out) {
            if (s.empty()) return false;
            char last = s.back();
            if (last != 'd' && last != 'D') return false;
            std::string num_str = s.substr(0, s.size() - 1);
            try {
                size_t pos;
                out = std::stod(num_str, &pos);
                return pos == num_str.size();
            }
            catch (...) {
                return false;
            }
        }

        bool parseBinULL(const std::string& s, unsigned long long& out, std::string& raw_bin) {
            if (s.size() < 2) return false;
            if (s[0] != '0' || (s[1] != 'b' && s[1] != 'B')) return false;
            raw_bin = s.substr(2);
            try {
                size_t pos;
                out = std::stoull(raw_bin, &pos, 2);
                return pos == raw_bin.size();
            }
            catch (...) {
                return false;
            }
        }

        bool parseString(const std::string& s, std::string& out) {
            if (s.size() < 2) return false;
            if (s.front() != '"' || s.back() != '"') return false;
            out = s.substr(1, s.size() - 2);
            return true;
        }

        std::vector<std::string> splitKeyValuePairs(const std::string& s) {
            std::vector<std::string> pairs;
            std::string current;
            bool inQuotes = false;
            for (char c : s) {
                if (c == '"') inQuotes = !inQuotes;
                if (c == ':' && !inQuotes) {
                    if (!current.empty()) {
                        pairs.push_back(current);
                        current.clear();
                    }
                }
                else {
                    current += c;
                }
            }
            if (!current.empty()) pairs.push_back(current);
            return pairs;
        }
    }

    bool parseLine(const std::string& line, DataStruct& ds) {
        size_t start = line.find_first_not_of(" \t");
        size_t end = line.find_last_not_of(" \t");
        if (start == std::string::npos || end == std::string::npos) return false;

        std::string trimmed = line.substr(start, end - start + 1);
        if (trimmed.size() < 4 ||
            trimmed.substr(0, 2) != "(:" ||
            trimmed.substr(trimmed.size() - 2) != ":)") return false;

        std::string inner = trimmed.substr(2, trimmed.size() - 4);
        auto pairs = splitKeyValuePairs(inner);

        DataStruct temp{};
        bool key1_found = false, key2_found = false, key3_found = false;

        for (const std::string& pair : pairs) {
            size_t space_pos = pair.find(' ');
            if (space_pos == std::string::npos) return false;

            std::string key = pair.substr(0, space_pos);
            std::string value = pair.substr(space_pos + 1);

            if (key == "key1") {
                if (!parseDoubleLiteral(value, temp.key1)) return false;
                key1_found = true;
            }
            else if (key == "key2") {
                if (!parseBinULL(value, temp.key2, temp.raw_bin)) return false;
                key2_found = true;
            }
            else if (key == "key3") {
                if (!parseString(value, temp.key3)) return false;
                key3_found = true;
            }
            else {
                return false;
            }
        }

        if (!key1_found || !key2_found || !key3_found) return false;
        ds = temp;
        return true;
    }

    std::istream& operator>>(std::istream& is, DataStruct& ds) {
        std::string line;
        while (std::getline(is, line)) {
            if (parseLine(line, ds)) return is;
        }
        is.setstate(std::ios::failbit);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
        std::ostringstream oss_key1;
        oss_key1 << std::fixed << ds.key1;
        std::string num_str = oss_key1.str();
        size_t dot_pos = num_str.find('.');
        if (dot_pos == std::string::npos) {
            num_str += ".0";
        }
        else {
            num_str.erase(num_str.find_last_not_of('0') + 1, std::string::npos);
            if (num_str.back() == '.') num_str += "0";
        }

        os << "(:key1 " << num_str << "d:key2 0b" << ds.raw_bin << ":key3 \"" << ds.key3 << "\":)";
        return os;
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.size() < b.key3.size();
    }
}
