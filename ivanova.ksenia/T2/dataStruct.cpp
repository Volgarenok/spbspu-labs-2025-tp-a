#include <iomanip>
#include <sstream>
#include <bitset>
#include "dataStruct.hpp"


namespace ivanova
{
  std::istream& operator>>(std::istream& in, dataStruct& ds)
  {
    std::string line;
    std::getline(in, line);

    if (!line.empty() && line.front() == '(' && line.back() == ')')
    {
      line = line.substr(1, line.size() - 2);
      std::istringstream iss(line);
      std::string pair;

      while (std::getline(iss, pair, ':'))
      {
        if (pair.empty()) continue;

        std::istringstream pairStream(pair);
        std::string key;
        pairStream >> key;

        if (key == "key1")
        {
          pairStream >> ds.key1;
        }
        else if (key == "key2")
        {
          std::string value;
          pairStream >> value;

          if (value.size() > 2 && (value.substr(0, 2) == "0b" || value.substr(0, 2) == "0B"))
          {
            try
            {
              ds.key2 = std::stoull(value.substr(2), nullptr, 2);
            }
            catch (...)
            {
              in.setstate(std::ios::failbit);
            }
          }
          else
          {
            in.setstate(std::ios::failbit);
          }
        }
        else if (key == "key3")
        {
          pairStream >> std::ws;
          std::getline(pairStream, ds.key3);
          if (!ds.key3.empty() && ds.key3.front() == '\"')
          {
            ds.key3 = ds.key3.substr(1);
          }
          if (!ds.key3.empty() && ds.key3.back() == '\"')
          {
            ds.key3.pop_back();
          }
        }
      }
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const dataStruct& ds)
  {
    out << "(:key1 " << std::scientific << std::setprecision(6) << ds.key1;

    out << ":key2 0b";
    std::bitset<64> bits(ds.key2);
    std::string binaryStr = bits.to_string();

    size_t firstOne = binaryStr.find('1');
    if (firstOne != std::string::npos)
    {
        out << binaryStr.substr(firstOne);
    }
    else
    {
        out << "0";
    }

    out << ":key3 \"" << ds.key3 << "\":)";

    out << std::defaultfloat;
    return out;
  }

  bool compareDataStructs(const ivanova::dataStruct& a, const ivanova::dataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }
    else if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }
    else
    {
      return a.key3.length() < b.key3.length();
    }
  }
}
