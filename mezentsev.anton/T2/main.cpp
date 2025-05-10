#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>
#include "data.h"

int main()
{
  std::vector< mezentsev::Data > data;
  while(!std::cin.eof())
  {
    std::copy
    (
      std::istream_iterator< mezentsev::Data >(std::cin),
      std::istream_iterator< mezentsev::Data >(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end(), mezentsev::compare);
  std::copy
  (
    std::begin(data),
    std::end(data),
    std::ostream_iterator< mezentsev::Data >(std::cout, "\n")
  );
  return 0;
}
