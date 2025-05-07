#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include "data_struct.hpp"
int main()
{
  using data_st = evstyunichev::DataStruct;
  using i_it_ds = std::istream_iterator< data_st >;
  using o_it_ds = std::ostream_iterator< data_st >;
  std::vector< data_st > data;
  while(!std::cin.eof())
  {
    std::copy(i_it_ds(std::cin), i_it_ds(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), evstyunichev::comparator);
  std::copy(std::begin(data), std::end(data), o_it_ds(std::cout, "\n")
  );
  return 0;
}
