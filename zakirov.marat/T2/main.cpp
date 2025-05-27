#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "stream_guardian.hpp"
#include "structures.hpp"

int main()
{
  zakirov::Guardian output_primary(std::cout);
  zakirov::Guardian input_primary(std::cin);

  std::vector< zakirov::Data > array;
  while (!std::cin.eof())
  {
    if (std::cin.fail())
    {
      std::cin.clear();
    }

    std::copy(
      std::istream_iterator< zakirov::Data >(std::cin),
      std::istream_iterator< zakirov::Data >(),
      std::back_inserter(array)
    );
  }
  std::sort(array.begin(), array.end());
  std::copy(
    array.begin(),
    array.end(),
    std::ostream_iterator< zakirov::Data >(std::cout, "\n")
  );
}
