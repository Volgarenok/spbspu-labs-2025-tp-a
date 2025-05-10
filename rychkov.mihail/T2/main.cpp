#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits>
#include <iomanip>

#include "data_struct.hpp"

namespace rychkov
{
  struct ds_compare
  {
    bool operator()(const DataStruct& lhs, const DataStruct& rhs)
    {
      if (lhs.key1 != rhs.key1)
      {
        return lhs.key1 < rhs.key1;
      }
      else if (lhs.key2 != rhs.key2)
      {
        return lhs.key2 < rhs.key2;
      }
      return lhs.key3.length() < rhs.key3.length();
    }
  };
}

int main()
{
  std::vector< rychkov::DataStruct > values;
  while (!std::cin.eof() && !std::cin.bad())
  {
    using Iter = std::istream_iterator< rychkov::DataStruct >;
    std::copy(Iter(std::cin), Iter(), std::back_inserter(values));
    if (!std::cin.eof() && !std::cin.bad())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(values.begin(), values.end(), rychkov::ds_compare());

  std::cout << std::setprecision(1);
  std::copy(values.begin(), values.end(), std::ostream_iterator< rychkov::DataStruct >(std::cout, "\n"));
}
