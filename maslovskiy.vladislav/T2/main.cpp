#include <iostream>
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>
#include "dataStruct.hpp"
#include "streamGuard.hpp"

int main()
{
  using namespace maslovskiy;
  std::vector< DataStruct > data; 
  while (!std::cin.eof())
  {
    DataStruct temp;
    if (std::cin >> temp)
    {
      data.push_back(temp);
    }
    else if (!std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >{ std::cout, "\n" });

  return 0;
}
