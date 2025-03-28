#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data.hpp"
int main()
{
  std::vector< kiselev::DataStruct > data;
  while(!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< kiselev::DataStruct >(std::cin),
      std::istream_iterator< kiselev::DataStruct >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), kiselev::compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< kiselev::DataStruct >(std::cout, "\n")
  );
  return 0;
}
