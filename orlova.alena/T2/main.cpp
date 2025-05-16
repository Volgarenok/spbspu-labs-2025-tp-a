#include <algorithm>
#include <limits>
#include "DataStruct.h"

bool compareDataStructs(const orlova::DataStruct& a, const orlova::DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  else if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  else
  {
    return a.key3.length() < b.key3.length();
  }
}

int main()
{
  using orlova::DataStruct;
  using istreamDS = std::istream_iterator< DataStruct >;
  using ostreamDS = std::ostream_iterator< DataStruct >;

  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    DataStruct temp;
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }

    std::copy(istreamDS{ std::cin }, istreamDS{}, std::back_inserter(data));
  }
  std::sort(data.begin(), data.end(), compareDataStructs);
  std::copy(std::begin(data), std::end(data), ostreamDS(std::cout, "\n"));
  return 0;
}
