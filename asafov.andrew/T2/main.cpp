#include "datastruct.h"
#include <algorithm>
#include <vector>
#include <limits>
#include <iterator>

int main()
{//(:key1 0b0:key2 #c(0.5 -0.5):key3 "Data":)
  std::vector<asafov::DataStruct> data;
  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator<asafov::DataStruct>(std::cin),
      std::istream_iterator<asafov::DataStruct>(),
      std::back_inserter(data)
    );
    if (!std::cin)
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::cout << data.size();
  std::sort(data.begin(), data.end(), asafov::cmpDataStruct);
  std::copy(
    std::begin(data),
    std::end(data),
    std::ostream_iterator<asafov::DataStruct>(std::cout, "\n")
  );
  return 0;
}
