#include <algorithm>
#include <limits>
#include <iterator>
#include <iostream>
#include <vector>

#include "DataStruct.hpp"

int main()
{
  using namespace shramko;
  using oIterator = std::ostream_iterator< dataStruct >;

  std::vector< dataStruct > data;

  try
  {
    dataStruct ds;
    while (true)
    {
      std::cin >> ds;
      if (std::cin.eof())
      {
        break;
      }
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        continue;
      }
      data.push_back(ds);
    }

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(), oIterator(std::cout, "\n"));
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
