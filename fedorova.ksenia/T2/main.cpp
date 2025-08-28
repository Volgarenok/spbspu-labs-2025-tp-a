#include <iostream>
#include <limits>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.h"

int main()
{
  std::vector< fedorova::DataStruct > vector;

  while (!std::cin.eof())
  {
    std::copy(
      std::istream_iterator< fedorova::DataStruct >(std::cin),
      std::istream_iterator< fedorova::DataStruct >(),
      std::back_inserter(vector)
    );
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  std::sort(vector.begin(), vector.end(), fedorova::compareDataStruct);

  std::copy(
    vector.begin(),
    vector.end(),
    std::ostream_iterator< fedorova::DataStruct >(std::cout, "\n")
  );

  return EXIT_SUCCESS;
}
