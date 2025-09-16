#ifndef UTILS_HPP
#define UTILS_HPP
#include <vector>
#include <string>
#include "polygon.hpp"

namespace kazak {
  namespace utils {

    bool parsePolygon(const std::string & line, Polygon & poly);
    void boundingBox(const std::vector<Polygon> & polygons, Point & minP, Point & maxP);
    bool polygonInFrame(const Polygon & poly, const Point & minP, const Point & maxP);

  }
}

#endif
