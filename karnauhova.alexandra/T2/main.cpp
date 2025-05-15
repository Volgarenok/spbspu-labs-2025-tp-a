#include <vector>
#include <iterator>
#include <limits>
#include <iostream>
#include <algorithm>
#include "data_struct.hpp"

int main()
{
  using namespace karnauhova;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< karnauhova::DataStruct >(std::cin),
      std::istream_iterator< karnauhova::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< karnauhova::DataStruct >(std::cout, "\n")
  );
}
