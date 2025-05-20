#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data_struct.hpp"
int main()
{
  std::vector< brevnov::DataStruct > data;
  while(!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< brevnov::DataStruct >(std::cin),
      std::istream_iterator< brevnov::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), brevnov::compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< brevnov::DataStruct >(std::cout, "\n")
  );
  return 0;
}
