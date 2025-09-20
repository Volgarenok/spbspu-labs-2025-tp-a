#include <algorithm>
#include <limits>
#include <iterator>
#include <iostream>
#include <vector>
#include "DataStruct.h"
int main()
{
  using dataStruct = khokhryakova::DataStruct;
  using istreamIterator = std::istream_iterator<dataStruct>;
  using ostreamIterator = std::ostream_iterator<dataStruct>;

  std::vector<dataStruct> data;
  while (!std::cin.eof())
  {
    std::copy(istreamIterator(std::cin), istreamIterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), ostreamIterator(std::cout, "\n"));
  return 0;
}
