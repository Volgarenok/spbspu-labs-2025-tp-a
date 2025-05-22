#include "geometry-utils.h"

double savintsev::calc_polygon_area(const Polygon a)
{
  size_t n = a.points.size();
  if (n < 3)
  {
    return 0.0;
  }

  double area = 0.0;
  for (size_t i = 0; i < n; ++i)
  {
    size_t j = (i + 1) % n;
    area += (a.points[i].x * a.points[j].y) - (a.points[j].x * a.points[i].y);
  }
  return 0.5 * abs(area);
}

bool savintsev::is_lines_int(const Point m1, const Point m2, const Point n1, const Point n2)
{
  if (!is_bbox_int(m1, m2, n1, n2))
  {
    return false;
  }

  int d1 = (n2.x - n1.x) * (m1.y - n1.y) - (n2.y - n1.y) * (m1.x - n1.x);
  int d2 = (n2.x - n1.x) * (m2.y - n1.y) - (n2.y - n1.y) * (m2.x - n1.x);
  int d3 = (m2.x - m1.x) * (n1.y - m1.y) - (m2.y - m1.y) * (n1.x - m1.x);
  int d4 = (m2.x - m1.x) * (n2.y - m1.y) - (m2.y - m1.y) * (n2.x - m1.x);

  if ((d1 * d2 < 0) && (d3 * d4 < 0))
  {
    return true;
  }

  if (is_on_segment(m1, n1, n2) || is_on_segment(m2, n1, n2))
  {
    return true;
  }
  if (is_on_segment(n1, m1, m2) || is_on_segment(n2, m1, m2))
  {
    return true;
  }

  return false;
}

bool savintsev::is_bbox_int(const Point a1, const Point a2, const Point b1, const Point b2)
{
  bool c1 = std::max(a1.x, a2.x) < std::min(b1.x, b2.x);
  bool c2 = std::min(a1.x, a2.x) > std::max(b1.x, b2.x);
  bool c3 = std::max(a1.y, a2.y) < std::min(b1.y, b2.y);
  bool c4 = std::min(a1.y, a2.y) > std::max(b1.y, b2.y);
  return !(c1 || c2 || c3 || c4);
}

bool savintsev::is_on_segment(const Point p, const Point p1, const Point p2)
{
  int cross = (p2.x - p1.x) * (p.y - p1.y) - (p2.y - p1.y) * (p.x - p1.x);
  if (cross != 0)
  {
    return false;
  }

  return p.x >= std::min(p1.x, p2.x) && p.x <= std::max(p1.x, p2.x) &&
          p.y >= std::min(p1.y, p2.y) && p.y <= std::max(p1.y, p2.y);
}
