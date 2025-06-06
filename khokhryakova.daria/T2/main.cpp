#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <limits>
#include "DataStruct.h"
int main()
{
  std::vector<khokhryakova::DataStruct> data;
  while (!std::cin.eof())
  {
    khokhryakova::DataStruct temp;
    if (std::cin >> temp)
    {
      std::copy(&temp, &temp + 1, std::back_inserter(data));
    }
    else if (!std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(
    data.begin(),
    data.end(),
    std::ostream_iterator<khokhryakova::DataStruct>(std::cout, "\n")
  );
  return 0;
}
