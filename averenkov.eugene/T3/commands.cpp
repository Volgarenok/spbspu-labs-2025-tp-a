#include "commands.hpp"

namespace averenkov
{
  bool RightAngleChecker::operator()(const Polygon& poly) const
  {
    const auto& pts = poly.points;
    if (pts.size() < 3)
    {
      return false;
    }

    for (size_t i = 0; i < pts.size(); ++i)
    {
      size_t j = (i + 1) % pts.size();
      size_t k = (i + 2) % pts.size();

      int dx1 = pts[j].x - pts[i].x;
      int dy1 = pts[j].y - pts[i].y;
      int dx2 = pts[k].x - pts[j].x;
      int dy2 = pts[k].y - pts[j].y;

      if (dx1 * dx2 + dy1 * dy2 == 0)
      {
        return true;
      }
    }
    return false;
  }

  VertexCount::VertexCount(size_t num):
    num_(num)
  {
  }

  bool VertexCount::operator()(const Polygon& poly) const
  {
    return poly.points.size() == num_;
  }

  bool PolygonEqual::operator()(const Polygon& a, const Polygon& b) const
  {
    if (a.points.size() != b.points.size())
    {
      return false;
    }
    return std::is_permutation(a.points.cbegin(), a.points.cend(), b.points.cbegin());
  }

  double calculateArea(const Polygon& poly)
  {
    double area = 0.0;
    const auto& pts = poly.points;
    for (size_t i = 0; i < pts.size(); ++i)
    {
      size_t j = (i + 1) % pts.size();
      area += (pts[i].x * pts[j].y) - (pts[j].x * pts[i].y);
    }
    return std::abs(area) / 2.0;
  }

  double AreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
  {
    double sum = 0.0;
    for (const auto& poly: polygons)
    {
      sum += calculateArea(poly);
    }
    return sum;
  }

  double EvenAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
  {
    double sum = 0.0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() % 2 == 0)
      {
        sum += calculateArea(poly);
      }
    }
    return sum;
  }

  double OddAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
  {
    double sum = 0.0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() % 2 != 0)
      {
        sum += calculateArea(poly);
      }
    }
    return sum;
  }

  double NumVertexAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
  {
    double sum = 0.0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() == num)
      {
        sum += calculateArea(poly);
      }
    }
    return sum;
  }

  double MeanAreaCalculator::operator()(const std::vector< Polygon >& polygons) const
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MEAN calculation");
    }
    return AreaSumCalculator()(polygons) / polygons.size();
  }

  void MaxAreaFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MAX calculation");
    }
    double maxArea = calculateArea(polygons[0]);
    for (const auto& poly: polygons)
    {
      double area = calculateArea(poly);
      if (area > maxArea)
      {
        maxArea = area;
      }
    }
    averenkov::iofmtguard guard(out);
    out << std::fixed << std::setprecision(1) << maxArea;
  }

  void MaxVertexCountFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MAX calculation");
    }
    size_t maxVertices = polygons[0].points.size();
    for (const auto& poly: polygons)
    {
      size_t vertices = poly.points.size();
      if (vertices > maxVertices)
      {
        maxVertices = vertices;
      }
    }
    out << maxVertices;
  }

  void MinAreaFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MIN calculation");
    }
    double minArea = calculateArea(polygons[0]);
    for (const auto& poly: polygons)
    {
      double area = calculateArea(poly);
      if (area < minArea)
      {
        minArea = area;
      }
    }
    averenkov::iofmtguard guard(out);
    out << std::fixed << std::setprecision(1) << minArea;
  }

  void MinVertexCountFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
  {
    if (polygons.empty())
    {
      throw std::runtime_error("No polygons for MIN calculation");
    }
    size_t minVertices = polygons[0].points.size();
    for (const auto& poly: polygons)
    {
      size_t vertices = poly.points.size();
      if (vertices < minVertices)
      {
        minVertices = vertices;
      }
    }
    out << minVertices;
  }

  size_t EvenCounter::operator()(const std::vector< Polygon >& polygons) const
  {
    size_t count = 0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() % 2 == 0)
      {
        ++count;
      }
    }
    return count;
  }

  size_t OddCounter::operator()(const std::vector< Polygon >& polygons) const
  {
    size_t count = 0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() % 2 != 0)
      {
        ++count;
      }
    }
    return count;
  }

  size_t NumVertexCounter::operator()(const std::vector< Polygon >& polygons) const
  {
    size_t count = 0;
    for (const auto& poly: polygons)
    {
      if (poly.points.size() == num)
      {
        ++count;
      }
    }
    return count;
  }

  void printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    in >> param;
    std::map<std::string, std::function<double(const std::vector< Polygon >&)>> commands;
    commands["EVEN"] = EvenAreaSumCalculator();
    commands["ODD"] = OddAreaSumCalculator();
    commands["MEAN"] = MeanAreaCalculator();
    auto it = commands.find(param);
    if (it != commands.end())
    {
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << it->second(polygons);
    }
    else
    {
      size_t num = std::stoul(param);
      if (num < 3)
      {
        throw std::invalid_argument("Invalid input");
      }
      double sum = NumVertexAreaSumCalculator{ num }(polygons);
      averenkov::iofmtguard guard(out);
      out << std::fixed << std::setprecision(1) << sum;
    }
  }

  void printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    in >> param;
    std::map<std::string, std::function<void(const std::vector< Polygon >&, std::ostream&)>> commands;
    commands["AREA"] = MaxAreaFinder();
    commands["VERTEXES"] = MaxVertexCountFinder();

    auto it = commands.find(param);
    if (it != commands.end())
    {
      it->second(polygons, out);
    }
    else
    {
      throw std::runtime_error("Invalid MAX parameter");
    }
  }

  void printMinValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    in >> param;

    std::map<std::string, std::function<void(const std::vector< Polygon >&, std::ostream&)>> commands;
    commands["AREA"] = MinAreaFinder();
    commands["VERTEXES"] = MinVertexCountFinder();

    auto it = commands.find(param);
    if (it != commands.end())
    {
      it->second(polygons, out);
    }
    else
    {
      throw std::runtime_error("Invalid MIN parameter");
    }
  }

  void printCountOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    std::string param;
    in >> param;
    std::map< std::string, std::function< size_t(const std::vector< Polygon >&) > > commands;
    commands["EVEN"] = EvenCounter();
    commands["ODD"] = OddCounter();
    if (std::isdigit(param[0]))
    {
      size_t num = std::stoull(param);
      if (num < 3)
      {
        throw std::invalid_argument("Error in input");
      }
      out << std::count_if(polygons.begin(), polygons.end(), VertexCount(num));
      return;
    }
    out << commands.at(param)(polygons);
  }

  void printPermsCnt(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
  {
    Polygon target;
    if (!(in >> target))
    {
      throw std::runtime_error("Invalid PERMS parameter");
    }
    PolygonEqual comparator;
    auto func = std::bind(&PolygonEqual::operator(), &comparator, std::placeholders::_1, target);
    size_t count = std::count_if(polygons.begin(), polygons.end(), func);
    out << count;
  }

  void printRightsCnt(const std::vector< Polygon >& polygons, std::ostream& out)
  {
    size_t count = std::count_if(polygons.begin(), polygons.end(), RightAngleChecker());
    out << count;
  }

  void printRmEcho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out)
  {
    Polygon target;
    if (!(in >> target))
    {
      throw std::runtime_error("Invalid RMECHO parameter");
    }

    size_t removed = 0;
    PolygonEqual comparator;
    auto func = std::bind(&PolygonEqual::operator(), &comparator, std::placeholders::_1, target);
    auto newEnd = std::unique(polygons.begin(), polygons.end(), func);

    removed = std::distance(newEnd, polygons.end());
    polygons.erase(newEnd, polygons.end());
    out << removed;
  }
}
