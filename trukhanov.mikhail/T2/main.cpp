#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "data_struct.hpp"

using trukhanov::DataStruct;

namespace
{
  bool compareDataStructs(const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1)
    {
      return a.key1 < b.key1;
    }
    if (a.key2 != b.key2)
    {
      return a.key2 < b.key2;
    }
    return a.key3.length() < b.key3.length();
  }
}

int main()
{
  std::vector<DataStruct> data;
  std::string line;

  while (!std::cin.eof())
  {
    std::getline(std::cin, line);
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);
    DataStruct ds;

    if (iss >> ds)
    {
      data.push_back(ds);
    }
  }

  std::sort(data.begin(), data.end(), compareDataStructs);
  std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}

