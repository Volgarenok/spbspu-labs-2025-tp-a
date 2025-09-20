#include "functional.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <numeric>

namespace nehvedovich
{
  AreaIt maxAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp)
  {
    return std::max_element(first, last, comp);
  }

  AreaIt minAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp)
  {
    return std::min_element(first, last, comp);
  }

  VertIt maxVertElement(VertIt first, VertIt last, const VerticesComparator &comp)
  {
    return std::max_element(first, last, comp);
  }

  VertIt minVertElement(VertIt first, VertIt last, const VerticesComparator &comp)
  {
    return std::min_element(first, last, comp);
  }

  bool is_congruent_by_shift(const Polygon &candidate, const Polygon &reference, std::size_t shift)
  {
    if (candidate.points.size() != reference.points.size())
    {
      return false;
    }

    std::vector< Point > rotated(candidate.points.size());
    std::rotate_copy(candidate.points.begin(), candidate.points.begin() + shift, candidate.points.end(),
                     rotated.begin());

    int dx = reference.points[0].x - rotated[0].x;
    int dy = reference.points[0].y - rotated[0].y;

    std::vector< Point > translated(rotated.size());
    std::transform(rotated.begin(), rotated.end(), translated.begin(), TranslatePoint(dx, dy));

    return std::equal(translated.begin(), translated.end(), reference.points.begin());
  }

  unsigned count_translation_congruent(const PolyVector &polygons, const Polygon &reference)
  {
    return std::count_if(polygons.begin(), polygons.end(), IsTranslationCongruent(reference));
  }
  std::vector< Point > AccumulatePoints::operator()(std::vector< Point > acc, const Polygon &poly) const
  {
    acc.insert(acc.end(), poly.points.begin(), poly.points.end());
    return acc;
  }

  bool CompareX::operator()(const Point &a, const Point &b) const
  {
    return a.x < b.x;
  }

  bool CompareY::operator()(const Point &a, const Point &b) const
  {
    return a.y < b.y;
  }

  BoundingBox::BoundingBox(const std::vector< Polygon > &polygons)
  {
    std::vector< Point > all_points =
        std::accumulate(polygons.begin(), polygons.end(), std::vector< Point > {}, AccumulatePoints());
    if (all_points.empty())
      throw std::runtime_error("No points in polygons");
    auto minmax_x = std::minmax_element(all_points.begin(), all_points.end(), CompareX());
    auto minmax_y = std::minmax_element(all_points.begin(), all_points.end(), CompareY());
    minX = minmax_x.first->x;
    maxX = minmax_x.second->x;
    minY = minmax_y.first->y;
    maxY = minmax_y.second->y;
  }

  BoundingBox::ContainsPoint::ContainsPoint(int minX, int maxX, int minY, int maxY):
    minX(minX),
    maxX(maxX),
    minY(minY),
    maxY(maxY)
  {}

  bool BoundingBox::ContainsPoint::operator()(const Point &p) const
  {
    return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
  }

  bool BoundingBox::contains(const Polygon &poly) const
  {
    return std::all_of(poly.points.begin(), poly.points.end(), ContainsPoint(minX, maxX, minY, maxY));
  }

}
