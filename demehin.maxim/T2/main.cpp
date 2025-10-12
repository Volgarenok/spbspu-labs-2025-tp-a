#include <vector>
#include <iterator>
#include <iostream>
#include <limits>
#include <algorithm>
#include "data_struct.hpp"

int main()
{
  using demehin::DataStruct;
  using istrIter = std::istream_iterator< DataStruct >;
  using ostrIter = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(istrIter(std::cin), istrIter(), std::back_inserter(data));

    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());

  std::copy(std::begin(data), std::end(data), ostrIter(std::cout, "\n"));
}


