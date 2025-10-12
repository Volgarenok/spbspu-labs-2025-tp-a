#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "dataStruct.hpp"

int main()
{
  using namespace duhanina;
  using istIter = std::istream_iterator< DataStruct >;
  using ostIter = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(istIter(std::cin), istIter(), std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), ostIter(std::cout, "\n"));
  return 0;
}

