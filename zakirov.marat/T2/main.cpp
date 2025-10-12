#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "processing_structures.hpp"

int main()
{
  using data_in_s_it = std::istream_iterator< zakirov::Data >;
  using data_out_s_it = std::ostream_iterator< zakirov::Data >;
  std::vector< zakirov::Data > array;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }

    std::copy(data_in_s_it(std::cin), data_in_s_it(), std::back_inserter(array));
  }
  std::sort(array.begin(), array.end());
  std::copy(array.begin(), array.end(), data_out_s_it(std::cout, "\n"));
}
