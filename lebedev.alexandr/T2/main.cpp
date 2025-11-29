#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <limits>
#include <delimiter.hpp>
#include "ioData.hpp"
#include "dataStruct.hpp"
#include "sortDataVector.hpp"

int main()
{
  using lebedev::DataStruct;
  std::vector< DataStruct > dataVect;
  using istream_iter = std::istream_iterator< DataStruct >;
  while (!std::cin.eof())
  {
    std::copy(istream_iter{ std::cin }, istream_iter{}, std::back_inserter(dataVect));
    if (std::cin.fail() && !std::cin.eof())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  lebedev::dataSort(dataVect.begin(), dataVect.end());
  std::copy(dataVect.begin(), dataVect.end(), std::ostream_iterator< DataStruct >{ std::cout, "\n" });
}
