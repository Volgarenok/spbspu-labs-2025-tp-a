#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>
#include "data_struct.hpp"

int main()
{
  using namespace smirnov;
  using DataInputIt = std::istream_iterator< DataStruct >;
  using DataOutputIt = std::ostream_iterator< DataStruct >;
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(DataInputIt(std::cin), DataInputIt(), std::back_inserter(data));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), compare);
  std::copy(data.begin(), data.end(), DataOutputIt(std::cout, "\n"));
}
