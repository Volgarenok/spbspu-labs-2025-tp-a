#include "commands.hpp"

averenkov::AngleCheckHelper::AngleCheckHelper(const std::vector< Point >& points):
  pts(points),
  idx(0)
{}

bool averenkov::AngleCheckHelper::operator()(const Point&)
{
  size_t j = (idx + 1) % pts.size();
  size_t k = (idx + 2) % pts.size();

  int dx1 = pts[j].x - pts[idx].x;
  int dy1 = pts[j].y - pts[idx].y;
  int dx2 = pts[k].x - pts[j].x;
  int dy2 = pts[k].y - pts[j].y;

  bool hasRightAngle = (dx1 * dx2 + dy1 * dy2 == 0);
  idx++;
  return hasRightAngle;
}

bool averenkov::RightAngleChecker::operator()(const Polygon& poly) const
{
  const auto& pts = poly.points;
  if (pts.size() < 3)
  {
    return false;
  }
  AngleCheckHelper helper(pts);
  return std::any_of(pts.begin(), pts.end(), helper);
}

averenkov::VertexCount::VertexCount(size_t num):
  num_(num)
{
}

bool averenkov::VertexCount::operator()(const Polygon& poly) const
{
  return poly.points.size() == num_;
}

bool averenkov::PolygonEqual::operator()(const Polygon& a, const Polygon& b) const
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }
  return std::is_permutation(a.points.cbegin(), a.points.cend(), b.points.cbegin());
}

double averenkov::calculateArea(const Polygon& poly)
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

double averenkov::AreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  double sum = 0.0;
  for (const auto& poly: polygons)
  {
    sum += calculateArea(poly);
  }
  return sum;
}

double averenkov::EvenAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
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

double averenkov::OddAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
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


double averenkov::NumVertexAreaSumCalculator::operator()(const std::vector< Polygon >& polygons) const
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

double averenkov::MeanAreaCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MEAN calculation");
  }
  return AreaSumCalculator()(polygons) / polygons.size();
}

void averenkov::MaxAreaFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MAX calculation");
  }
  double maxArea = calculateArea(polygons[0]);
  for (const auto& poly : polygons)
  {
    double area = calculateArea(poly);
    if (area > maxArea)
    {
      maxArea = area;
    }
  }
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1) << maxArea;
}

void averenkov::MaxVertexCountFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MAX calculation");
  }
  size_t maxVertices = polygons[0].points.size();
  for (const auto& poly : polygons)
  {
    size_t vertices = poly.points.size();
    if (vertices > maxVertices)
    {
      maxVertices = vertices;
    }
  }
  out << maxVertices;
}

void averenkov::MinAreaFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MIN calculation");
  }
  double minArea = calculateArea(polygons[0]);
  for (const auto& poly : polygons)
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

void averenkov::MinVertexCountFinder::operator()(const std::vector< Polygon >& polygons, std::ostream& out) const
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

bool averenkov::IsEven::operator()(const Polygon& poly) const
{
  return poly.points.size() % 2 == 0;
}

bool averenkov::IsOdd::operator()(const Polygon& poly) const
{
  return poly.points.size() % 2 != 0;
}

size_t averenkov::EvenCounter::operator()(const std::vector< Polygon >& polygons) const
{
  return std::count_if(polygons.begin(), polygons.end(), IsEven());
}

size_t averenkov::OddCounter::operator()(const std::vector< Polygon >& polygons) const
{
  return std::count_if(polygons.begin(), polygons.end(), IsOdd());
}

size_t averenkov::NumVertexCounter::operator()(const std::vector< Polygon >& polygons) const
{
  return std::count_if(polygons.begin(), polygons.end(), VertexCount(num));
}

void averenkov::printAreaSum(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
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

void averenkov::printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
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

void averenkov::printMinValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
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

void averenkov::printCountOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
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

void averenkov::printPermsCnt(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
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

void averenkov::printRightsCnt(const std::vector< Polygon >& polygons, std::ostream& out)
{
  size_t count = std::count_if(polygons.begin(), polygons.end(), RightAngleChecker());
  out << count;
}

void averenkov::printRmEcho(std::istream& in, std::vector< Polygon >& polygons, std::ostream& out)
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

