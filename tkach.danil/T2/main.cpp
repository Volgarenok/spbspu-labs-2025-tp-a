#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "data_struct.hpp"
#include "io_actions.hpp"

namespace
{
  bool comp(const tkach::DataStruct& data1, const tkach::DataStruct& data2)
  {
    if (data1.key1 != data2.key1)
    {
      return data1.key1 < data2.key1;
    }
    else if (data1.key2 != data2.key2)
    {
      return data1.key2 < data2.key2;
    }
    return data1.key3.length() < data2.key3.length();
  }
}

int main()
{
  std::vector< tkach::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< tkach::DataStruct >(std::cin),
      std::istream_iterator< tkach::DataStruct >(),
      std::back_inserter(data)
    );
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), comp);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< tkach::DataStruct >(std::cout, "\n")
  );
  return 0;
}
