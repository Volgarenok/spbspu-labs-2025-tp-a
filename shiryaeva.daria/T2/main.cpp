#include "data_struct.hpp"
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <limits>

int main()
{
  using namespace shiryaeva;
  std::vector<DataStruct> data;

  while (!std::cin.eof())
  {
    using Iter = std::istream_iterator<DataStruct>;
    std::copy(Iter(std::cin), Iter(), std::back_inserter(data));

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  if (data.empty())
  {
    std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    return 0;
  }

  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n")
  );

  return 0;
}

