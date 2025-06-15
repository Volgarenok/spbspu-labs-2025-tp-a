#include <vector>
#include <iterator>
#include <limits>
#include <iostream>
#include <algorithm>
#include "data_struct.hpp"

int main()
{
  using namespace karnauhova;
  using istr_iterator = std::istream_iterator< karnauhova::DataStruct >;
  using ostr_iter = std::ostream_iterator< karnauhova::DataStruct >;
  std::vector< DataStruct > data;

  while (!std::cin.eof())
  {
    std::copy(istr_iterator(std::cin), istr_iterator(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy(std::begin(data), std::end(data), ostr_iter(std::cout, "\n"));
}
