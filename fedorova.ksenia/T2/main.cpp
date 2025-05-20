#include <iostream>
#include <limits>
#include "DataStruct.h"

bool compareDataStruct(const fedorova::DataStruct& a, const fedorova::DataStruct& b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }

  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }

  return a.key3.length() < b.key3.length();
}

int main() {
  using fedorova::DataStruct;
  using namespace fedorova;

  std::vector<DataStruct> data;

  while (!std::cin.eof())
  {
    DataStruct temp;
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::copy(
      std::istream_iterator< DataStruct >(std::cin),
      std::istream_iterator< DataStruct >(),
      std::back_inserter(data)
    );
  }

  std::sort(data.begin(), data.end(), compareDataStruct);

  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n")
  );

  return 0;
}
