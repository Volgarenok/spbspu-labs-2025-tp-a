#include "poligon.hpp"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <string>
#include <iostream>
#include <vector>
#include <streamGuard.hpp>
#include <delimiter.hpp>

namespace
{
  struct TriangleVectorCreator
  {
    const dribas::Point& firstPolygonPoint;
    const std::vector< dribas::Point >& allPolygonPoints;
    TriangleVectorCreator(const dribas::Point& firstPt, const std::vector< dribas::Point >& allPts):
      firstPolygonPoint(firstPt),
      allPolygonPoints(allPts)
    {}
    std::vector< dribas::Point > operator()(size_t index) const
    {
      return {firstPolygonPoint, allPolygonPoints[index], allPolygonPoints[index + 1]};
    }
  };

  struct TriangleAreaCalculator
  {
    double operator()(const std::vector<dribas::Point>& trianglePoints) const
    {
      if (trianglePoints.size() != 3) {
        return 0.0;
      }
      const dribas::Point& p1 = trianglePoints[0];
      const dribas::Point& p2 = trianglePoints[1];
      const dribas::Point& p3 = trianglePoints[2];
      return 0.5 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
    }
  };
}

std::istream& dribas::operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  StreamGuard guard(in);

  in >> DelimiterI{ '(' } >> point.x;
  in >> DelimiterI{ ';' } >> point.y;
  in >> DelimiterI{ ')' };

  return in;
}

std::istream& dribas::operator>>(std::istream& in, Poligon& plg)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  StreamGuard scope(in);
  size_t size = 0;
  if (!(in >> size) || size < 3) {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector< Point > temp(size);
  std::copy_n(std::istream_iterator< Point >(in), size, temp.begin());
  if (in) {
    plg.points = std::move(temp);
  }
  return in;
}

double dribas::getPoligonArea(const Poligon& poligon)
{
  if (poligon.points.size() < 3) {
    return 0.0;
  }

  const Point& firstPt = poligon.points[0];
  std::vector< size_t > indices(poligon.points.size() - 2);
  std::iota(indices.begin(), indices.end(), 1);

  std::vector< std::vector< dribas::Point > > triangles;
  triangles.reserve(indices.size());
  TriangleVectorCreator createTriangle(firstPt, poligon.points);
  std::transform(indices.begin(), indices.end(), std::back_inserter(triangles), createTriangle);

  std::vector< double > triangleAreas;
  triangleAreas.reserve(triangles.size());
  TriangleAreaCalculator calculateArea;
  std::transform(triangles.begin(), triangles.end(), std::back_inserter(triangleAreas), calculateArea);
  return std::abs(std::accumulate(triangleAreas.begin(), triangleAreas.end(), 0.0));
}
