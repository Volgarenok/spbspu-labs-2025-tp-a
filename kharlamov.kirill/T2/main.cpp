#include <algorithm>
#include <iterator>
#include <limits>
#include <vector>
#include "data.h"
int main()
{
  std::vector< kharlamov::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< kharlamov::DataStruct >(std::cin),
      std::istream_iterator< kharlamov::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), kharlamov::compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< kharlamov::DataStruct >(std::cout, "\n")
  );
  return 0;
}
