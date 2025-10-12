#include "FillVectorOfShapes.h"

#include <iterator>
#include <limits>

namespace voronina
{
  std::vector< Polygon > fillVectorOfShapes(std::ifstream& fin)
  {
    if (!fin.is_open())
    {
      throw std::invalid_argument("ERROR: Wrong name of file");
    }
    std::vector< Polygon > input;
    while (!fin.eof())
    {
      using IstreamIterator = std::istream_iterator< Polygon >;
      std::copy(IstreamIterator(fin), IstreamIterator(), std::back_inserter(input));
      if (fin.fail() && !fin.eof())
      {
        fin.clear();
        fin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    fin.close();
    return input;
  }
}
