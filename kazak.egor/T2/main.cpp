#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <limits>
#include "stream_guard.hpp"
#include "datastruct.hpp"

int main()
{
  using namespace kazak;
  using kazak::DataStruct;

  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    DataStruct ds;

    if (std::cin >> ds)
    {
      data.push_back(ds);
    }
    else
    {
      if (std::cin.eof())
      {
        break;
      }

      std::cin.clear();

      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return 0;
}


