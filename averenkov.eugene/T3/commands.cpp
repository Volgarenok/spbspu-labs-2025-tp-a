#include "commands.hpp"

double averenkov::acum(const std::vector< Polygon >& plgs, std::function< double(double accum, const Polygon& plg) > func)
{
  return std::accumulate(plgs.begin(), plgs.end(), 0.0, (func));
}

double averenkov::Prod::operator()(const averenkov::Point& a, const averenkov::Point& b) const
{
  return a.x * b.y - b.x * a.y;
}

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
{}

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

bool averenkov::AreaComparator::operator()(const Polygon& a, const Polygon& b) const
{
  return calculateArea(a) < calculateArea(b);
}

bool averenkov::VertexCountComparator::operator()(const Polygon& a, const Polygon& b) const
{
  return a.points.size() < b.points.size();
}

double averenkov::calculateArea(const Polygon& poly)
{
  const auto& points{ poly.points };
  std::vector< Point > shifted_points{ points.begin() + 1, points.end() };
  shifted_points.push_back(points[0]);
  double area = std::inner_product(points.begin(), points.end(), shifted_points.begin(), 0.0, std::plus< double >{}, Prod{});
  return std::abs(area) / 2.0;
}

double averenkov::AreaSumCalculator::operator()(const std::vector< Polygon >& plgs) const
{
  return acum(plgs, (*this));
}

double averenkov::AreaSumCalculator::operator()(double sum, const Polygon& poly) const
{
  return sum + calculateArea(poly);
}

double averenkov::EvenSumCalculator::operator()(double sum, const Polygon& poly) const
{
  if (poly.points.size() % 2 == 0)
  {
    return sum + calculateArea(poly);
  }
  return sum;
}

double averenkov::EvenSumCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  return acum(polygons, (*this));
}

double averenkov::OddSumCalculator::operator()(double sum, const Polygon& poly) const
{
  if (poly.points.size() % 2 != 0)
  {
    sum += calculateArea(poly);
  }
  return sum;
}

double averenkov::OddSumCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  return acum(polygons, (*this));
}

double averenkov::NumVertexCalculator::operator()(double sum, const Polygon& poly) const
{
  if (poly.points.size() == num)
  {
    return sum + calculateArea(poly);
  }
  return sum;
}

double averenkov::NumVertexCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  return acum(polygons, (*this));
}

double averenkov::MeanAreaCalculator::operator()(const std::vector< Polygon >& polygons) const
{
  if (polygons.empty())
  {
    throw std::runtime_error("No polygons for MEAN calculation");
  }
  return AreaSumCalculator()(polygons) / polygons.size();
}

averenkov::MaxAreaFinder::MaxAreaFinder(const std::vector< Polygon >& polygons):
  polygons_(polygons)
{}

averenkov::MaxVertexCountFinder::MaxVertexCountFinder(const std::vector< Polygon >& polygons):
  polygons_(polygons)
{}

averenkov::MinAreaFinder::MinAreaFinder(const std::vector< Polygon >& polygons):
  polygons_(polygons)
{}

averenkov::MinVertexCountFinder::MinVertexCountFinder(const std::vector< Polygon >& polygons):
  polygons_(polygons)
{}

std::ostream& averenkov::operator<<(std::ostream& out, const MaxAreaFinder& finder)
{
  if (finder.polygons_.empty())
  {
    throw std::runtime_error("Error");
  }
  auto it = std::max_element(finder.polygons_.begin(), finder.polygons_.end(), AreaComparator());
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1) << calculateArea(*it);
  return out;
}

std::ostream& averenkov::operator<<(std::ostream& out, const MaxVertexCountFinder& finder)
{
  if (finder.polygons_.empty())
  {
    throw std::runtime_error("No polygons for MAX calculation");
  }
  auto it = std::max_element(finder.polygons_.begin(), finder.polygons_.end(), VertexCountComparator());
  out << it->points.size();
  return out;
}

std::ostream& averenkov::operator<<(std::ostream& out, const MinAreaFinder& finder)
{
  if (finder.polygons_.empty())
  {
    throw std::runtime_error("No polygons for MIN calculation");
  }
  auto it = std::min_element(finder.polygons_.begin(), finder.polygons_.end(), AreaComparator());
  iofmtguard guard(out);
  out << std::fixed << std::setprecision(1) << calculateArea(*it);
  return out;
}

std::ostream& averenkov::operator<<(std::ostream& out, const MinVertexCountFinder& finder)
{
  if (finder.polygons_.empty())
  {
    throw std::runtime_error("No polygons for MIN calculation");
  }
  auto it = std::min_element(finder.polygons_.begin(), finder.polygons_.end(), VertexCountComparator());
  out << it->points.size();
  return out;
}

void averenkov::printMaxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  MaxAreaFinder finder(polygons);
  out << finder;
}

void averenkov::printMaxVertexCount(const std::vector< Polygon >& polygons, std::ostream& out)
{
  MaxVertexCountFinder finder(polygons);
  out << finder;
}

void averenkov::printMinArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  MinAreaFinder finder(polygons);
  out << finder;
}

void averenkov::printMinVertexCount(const std::vector< Polygon >& polygons, std::ostream& out)
{
  MinVertexCountFinder finder(polygons);
  out << finder;
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
  std::map< std::string, std::function< double(const std::vector< Polygon >&) > > commands;
  commands["EVEN"] = EvenSumCalculator();
  commands["ODD"] = OddSumCalculator();
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
    double sum = NumVertexCalculator{ num }(polygons);
    averenkov::iofmtguard guard(out);
    out << std::fixed << std::setprecision(1) << sum;
  }
}

void averenkov::printMaxValueOf(std::istream& in, const std::vector< Polygon >& polygons, std::ostream& out)
{
  std::string param;
  in >> param;
  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > commands;
  commands["AREA"] = printMaxArea;
  commands["VERTEXES"] = printMaxVertexCount;

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

  std::map< std::string, std::function< void(const std::vector< Polygon >&, std::ostream&) > > commands;
  commands["AREA"] = printMinArea;
  commands["VERTEXES"] = printMinVertexCount;

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
  auto func = std::bind(&PolygonEqual::operator(), std::addressof(comparator), std::placeholders::_1, target);
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
  auto func = std::bind(&PolygonEqual::operator(), std::addressof(comparator), std::placeholders::_1, target);
  auto newEnd = std::unique(polygons.begin(), polygons.end(), func);

  removed = std::distance(newEnd, polygons.end());
  polygons.erase(newEnd, polygons.end());
  out << removed;
}

