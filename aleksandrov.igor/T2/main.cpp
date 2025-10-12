#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include "data-struct.hpp"
#include "stream-guard.hpp"
#include "input-struct.hpp"
#include "output-struct.hpp"

int main()
{
  using namespace aleksandrov;

  std::vector< DataStruct > data;

  using InputDataIt = std::istream_iterator< DataStruct >;
  using OutputDataIt = std::ostream_iterator< DataStruct >;

  while (!std::cin.eof())
  {
    std::copy(InputDataIt(std::cin), InputDataIt(), std::back_inserter(data));
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(std::begin(data), std::end(data), OutputDataIt(std::cout, "\n"));
}

