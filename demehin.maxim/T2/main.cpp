#include "data_struct.hpp"
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

namespace
{
  using Data = demehin::DataStruct;
  void sortDataStrVector(std::vector< demehin::DataStruct >& data)
  {
    std::sort(data.begin(), data.end(), [](const Data& a, const Data& b)
    {
      if (a.key1 != b.key1)
      {
        return a.key1 < b.key1;
      }
      if (a.key2 != b.key2)
      {
        return a.key2 < b.key2;
      }
      return a.key3.size() < b.key3.size();
    });
  }

}

int main()
{
  std::vector< demehin::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< demehin::DataStruct >(std::cin),
      std::istream_iterator< demehin::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  sortDataStrVector(data);

  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< demehin::DataStruct >(std::cout, "\n")
  );
}


