#ifndef FILL_VECTOR_OF_SHAPES_H
#define FILL_VECTOR_OF_SHAPES_H

#include <fstream>
#include <exception>
#include <iterator>
#include <limits>
#include "Shapes.h"

namespace voronina
{
  inline std::vector< Polygon > fillVectorOfShapes(std::ifstream& fin)
  {
    if (!fin.is_open())
    {
      throw std::invalid_argument("ERROR: Wrong name of file");
    }
    std::vector< Polygon > input;
    while (!fin.eof())
    {
      using IstreamIterator = std::istream_iterator < Polygon >;
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
#endif
