#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "dataStruct.hpp"
#include "streamGuard.hpp"

int main()
{
  using namespace cherkasov;
  StreamGuard coutGuard(std::cout);
  StreamGuard cinGuard(std::cin);
  std::vector< DataStruct > data;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
      continue;
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< DataStruct >(std::cout, "\n")
  );
}
