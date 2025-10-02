#ifndef SHAPE_UTILS_HPP
#define SHAPE_UTILS_HPP
#include <string>
#include <iostream>
#include "shape.hpp"

namespace savintsev
{
  savintsev::Shape * createShape(std::istream & in, std::string name);
}

#endif
