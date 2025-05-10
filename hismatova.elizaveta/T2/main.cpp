#include "DataStruct.hpp"

int main()
{
  std::vector< DataStruct > dataVec;
  std::copy(std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(dataVec));
  dataVec.erase(
    std::remove_if(dataVec.begin(), dataVec.end(), [](const DataStruct& d)
    {
      return d.empty();
    }),
    dataVec.end()
  );
  std::sort(dataVec.begin(), dataVec.end(), [](const DataStruct& a, const DataStruct& b)
  {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (std::abs(a.key2) != std::abs(b.key2)) return std::abs(a.key2) < std::abs(b.key2);
    return a.key3.length() < b.key3.length();
  });
  std::copy(dataVec.begin(), dataVec.end(), std::ostream_iterator< DataStruct >(std::cout, "\n"));
  return 0;
}
