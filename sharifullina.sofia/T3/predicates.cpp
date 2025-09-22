#include "predicates.hpp"

namespace sharifullina::detail
{

  double calcPoints(const Point & p1, const Point & p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  bool cmpPoint(const Point & p1, const Point & p2)
  {
    if (p1.x != p2.x)
    {
      return p1.x < p2.x;
    }
    else
    {
      return p1.y < p2.y;
    }
  }

  IsPermutationPredicate::IsPermutationPredicate(const Polygon & t):
    target(t)
  {}

  bool IsPermutationPredicate::operator()(const Polygon & poly) const
  {
    if (poly.points.size() != target.points.size())
    {
      return false;
    }

    std::vector< Point > sortedLhs = poly.points;
    std::vector< Point > sortedRhs = target.points;

    std::sort(sortedLhs.begin(), sortedLhs.end(), cmpPoint);
    std::sort(sortedRhs.begin(), sortedRhs.end(), cmpPoint);

    return sortedLhs == sortedRhs;
  }

  LessAreaPredicate::LessAreaPredicate(double area):
    targetArea(area)
  {}

  bool LessAreaPredicate::operator()(const Polygon & poly) const
  {
    if (poly.points.size() < 3)
    {
      return 0.0 < targetArea;
    }
    Polygon rotPoly = poly;
    std::rotate(rotPoly.points.begin(), rotPoly.points.begin() + 1, rotPoly.points.end());
    auto it1 = poly.points.begin();
    auto it2 = poly.points.end();
    auto it3 = rotPoly.points.begin();
    double sum = std::inner_product(it1, it2, it3, 0.0, std::plus< double >(), calcPoints);
    double area = std::abs(sum) * 0.5;
    return area < targetArea;
  }
}
