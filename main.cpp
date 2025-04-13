#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>

struct DataStruct 
{
    long long key1;
    double key2;
    std::string key3;
};

template<typename T>
bool parseField(const std::string& field, const std::string& key, T& value) 
{
    size_t pos = field.find(key);
    if (pos == std::string::npos) return false;

    std::istringstream iss(field.substr(pos + key.length() + 1));
    if constexpr (std::is_same_v<T, long long>) 
    {
        std::string val;
        iss >> val;
        if (val.empty()) return false;

        if (val.length() > 2 && (val.substr(val.length() - 2) == "ll" || val.substr(val.length() - 2) == "LL")) 
        {
            val = val.substr(0, val.length() - 2);
        }

        try 
        {
            value = std::stoll(val);
            return true;
        }
        catch (...) 
        {
            return false;
        }
    }
    else if constexpr (std::is_same_v<T, double>) 
    {
        std::string val;
        iss >> val;
        if (val.empty()) return false;

        if (!val.empty() && (val.back() == 'd' || val.back() == 'D')) 
        {
            val.pop_back();
        }

        try 
        {
            value = std::stod(val);
            return true;
        }
        catch (...) 
        {
            return false;
        }
    }
    else if constexpr (std::is_same_v<T, std::string>) 
    {
        if (iss.peek() != '"') return false;
        iss.get(); 

        value.clear();
        char ch;
        while (iss.get(ch) && ch != '"') 
        {
            value += ch;
        }

        return ch == '"';
    }
    return false;
}

std::istream& operator>>(std::istream& in, DataStruct& ds) 
{
    std::string line;
    if (!std::getline(in, line)) return in;

    if (line.size() < 4 || line.substr(0, 2) != "(:" || line.substr(line.size() - 2) != ":)") 
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    line = line.substr(2, line.size() - 4);

    std::vector<std::string> fields;
    size_t start = 0;
    size_t end = line.find(':', start);

    while (end != std::string::npos) 
    {
        std::string field = line.substr(start, end - start);
        if (!field.empty()) 
        {
            fields.push_back(field);
        }
        start = end + 1;
        end = line.find(':', start);
    }

    std::string last_field = line.substr(start);
    if (!last_field.empty()) 
    {
        fields.push_back(last_field);
    }

    bool has_key1 = false, has_key2 = false, has_key3 = false;
    DataStruct temp;

    for (const auto& field : fields) 
    {
        if (field.find("key1") == 0) 
        {
            has_key1 = parseField(field, "key1", temp.key1);
        }
        else if (field.find("key2") == 0) 
        {
            has_key2 = parseField(field, "key2", temp.key2);
        }
        else if (field.find("key3") == 0) 
        {
            has_key3 = parseField(field, "key3", temp.key3);
        }
    }

    if (has_key1 && has_key2 && has_key3) 
    {
        ds = temp;
    }
    else 
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& ds) 
{
    out << "(:key1 " << ds.key1 << "ll:key2 " << std::fixed << std::setprecision(1)
        << ds.key2 << "d:key3 \"" << ds.key3 << "\":)";
    return out;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b) 
{
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main()
{
    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), compareDataStruct);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
