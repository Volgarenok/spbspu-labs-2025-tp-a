#ifndef FILL_VECTOR_OF_SHAPES_H
#define FILL_VECTOR_OF_SHAPES_H

#include <fstream>
#include <vector>
#include "Shapes.h"

namespace voronina
{
  std::vector< Polygon > fillVectorOfShapes(std::ifstream& fin);
}
#endif
