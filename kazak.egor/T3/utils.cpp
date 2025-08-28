#include <sstream>
#include <limits>
#include <algorithm>
#include "utils.hpp"

namespace kazak {
  namespace utils {

    bool parsePolygon(const std::string & line, Polygon & poly)
    {
      std::istringstream iss(line);
      size_t count = 0;
      if (!(iss >> count)) {
        return false;
      }
      if (count == 0) {
        return false;
      }
      poly.points.clear();
      for (size_t i = 0; i < count; ++i) {
        char ch1, ch2, ch3;
        int x, y;
        if (!(iss >> ch1 >> x >> ch2 >> y >> ch3)) {
          return false;
        }
        if (ch1 != '(' || ch2 != ';' || ch3 != ')') {
          return false;
        }
        poly.points.push_back(Point{x, y});
      }
      return true;
    }

    void boundingBox(const std::vector<Polygon> & polygons, Point & minP, Point & maxP)
    {
      minP.x = std::numeric_limits<int>::max();
      minP.y = std::numeric_limits<int>::max();
      maxP.x = std::numeric_limits<int>::min();
      maxP.y = std::numeric_limits<int>::min();

      for (const auto & poly : polygons) {
        for (const auto & p : poly.points) {
          minP.x = std::min(minP.x, p.x);
          minP.y = std::min(minP.y, p.y);
          maxP.x = std::max(maxP.x, p.x);
          maxP.y = std::max(maxP.y, p.y);
        }
      }
    }

    bool polygonInFrame(const Polygon & poly, const Point & minP, const Point & maxP)
    {
      for (const auto & p : poly.points) {
        if (p.x < minP.x || p.x > maxP.x || p.y < minP.y || p.y > maxP.y) {
          return false;
        }
      }
      return true;
    }

  }
}

