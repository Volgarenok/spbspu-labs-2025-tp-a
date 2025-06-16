#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>

#include "dataStruct.hpp"
#include "scopeGuard.hpp"

int main()
{
  using sharifullina::DataStruct;

  std::vector<DataStruct> data;

  while (!std::cin.eof())
  {
    DataStruct ds;
    if (std::cin >> ds)
    {
      data.push_back(ds);
    }
    else if (!std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n")
  );

  return 0;
}
