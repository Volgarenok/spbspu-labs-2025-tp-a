#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <string>
#include <vector>
#include "data.h"

int main()
{
  std::vector< ohantsev::Data > data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< ohantsev::Data >(std::cin),
      std::istream_iterator< ohantsev::Data >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(
    data.begin(),
    data.end(),
    [](const ohantsev::Data& a, const ohantsev::Data& b)
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
    }
  );
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator< ohantsev::Data >(std::cout, "\n")
  );
  return 0;
}
