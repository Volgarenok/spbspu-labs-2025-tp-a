#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include "data_struct.hpp"

namespace
{
  bool compareDataStructs(const petrov::DataStruct & lhs, const petrov::DataStruct & rhs);
  bool compareDataStructs(const petrov::DataStruct & lhs, const petrov::DataStruct & rhs)
  {
    if (lhs.key1 < rhs.key2)
    {
      return true;
    }
    else if (lhs.key2 < rhs.key2)
    {
      return true;
    }
    else if (lhs.key3.length() < rhs.key3.length())
    {
      return true;
    }
    return false;
  }
}

int main()
{
  using namespace petrov;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(1024, '\n');
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }
  std::sort(data.begin(), data.end(), compareDataStructs);
  std::copy(
      std::begin(data),
      std::end(data),
      std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
