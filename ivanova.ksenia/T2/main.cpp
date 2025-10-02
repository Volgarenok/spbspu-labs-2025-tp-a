#include <algorithm>
#include <limits>
#include <iterator>
#include <iostream>
#include <vector>

#include "dataStruct.hpp"

int main()
{
  using namespace ivanova;
  using iIterator = std::istream_iterator< dataStruct >;
  using oIterator = std::ostream_iterator< dataStruct >;

  std::vector< dataStruct > data;

  try
  {
    std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));

    while (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::copy(iIterator(std::cin), iIterator(), std::back_inserter(data));
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
