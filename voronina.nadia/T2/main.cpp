#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include <iostream>

int main()
{
  using voronina::DataStruct;
  std::vector<DataStruct> data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator<DataStruct>(std::cin),
      std::istream_iterator<DataStruct>(),
      std::back_inserter(data)
    );
    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() & ~std::ios::failbit);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  if (std::cin.bad())
  {
    std::cerr << "Fatal I/O error\n";
    return 1;
  }
  std::sort(std::begin(data), std::end(data), voronina::comparator);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator<DataStruct>(std::cout, "\n"));
  return 0;
}
