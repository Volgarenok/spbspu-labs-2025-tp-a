#include <limits>
#include <vector>
#include <iterator>
#include <algorithm>
#include <dataStruct.hpp>

int main()
{
  using i_iterator_t = std::istream_iterator< averenkov::DataStruct >;
  using o_iterator_t = std::ostream_iterator< averenkov::DataStruct >;

  std::vector< averenkov::DataStruct > data;
  while (!std::cin.eof())
  {
    std::copy(i_iterator_t(std::cin), i_iterator_t(), std::back_inserter(data));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), o_iterator_t(std::cout, "\n"));
  return 0;
}
