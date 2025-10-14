#include "commands.hpp"
#include <stream_guard.hpp>

namespace
{
  double triangleArea(const nikitin::Point& a, const nikitin::Point& b, const nikitin::Point& c)
  {
    return std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) / 2.0;
  }
  double polygonArea(const nikitin::Polygon& polygon)
  {
    if (polygon.points.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::vector< double > triangleAreas(polygon.points.size() - 2);
    const nikitin::Point pivot = polygon.points.front();
    auto functor = std::bind(triangleArea, pivot, std::placeholders::_1, std::placeholders::_2);
    auto pointB_it = std::next(polygon.points.begin());
    auto pointC_it = std::next(pointB_it);
    std::transform(pointB_it, std::prev(polygon.points.end()), pointC_it, triangleAreas.begin(), functor);
    return std::accumulate(triangleAreas.cbegin(), triangleAreas.cend(), 0.0);
  }
  double sumOfAreas(const std::vector< nikitin::Polygon >& polygons)
  {
    std::vector< double > sums(polygons.size());
    std::transform(polygons.begin(), polygons.end(), sums.begin(), polygonArea);
    return std::accumulate(sums.cbegin(), sums.cend(), 0.0);
  }
  std::size_t getVerticies(const nikitin::Polygon& polygon)
  {
    return polygon.points.size();
  }
  bool evenVerticies(const nikitin::Polygon& polygon)
  {
    return (polygon.points.size() % 2) == 0;
  }
  bool oddVerticies(const nikitin::Polygon& polygon)
  {
    return (polygon.points.size() % 2) == 1;
  }
  bool equalVerticies(std::size_t n, const nikitin::Polygon& polygon)
  {
    return n == polygon.points.size();
  }
  double evenArea(const std::vector< nikitin::Polygon >& polygons)
  {
    std::vector< nikitin::Polygon > filtred;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtred), evenVerticies);
    return sumOfAreas(filtred);
  }
  double oddArea(const std::vector< nikitin::Polygon >& polygons)
  {
    std::vector< nikitin::Polygon > filtred;
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtred), oddVerticies);
    return sumOfAreas(filtred);
  }
  double verticiesArea(const std::vector< nikitin::Polygon >& polygons, std::size_t n)
  {
    if (n < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< nikitin::Polygon > filtred;
    auto functor = std::bind(equalVerticies, n, std::placeholders::_1);
    std::copy_if(polygons.cbegin(), polygons.cend(), std::back_inserter(filtred), functor);
    return sumOfAreas(filtred);
  }
  double meanArea(const std::vector< nikitin::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    return sumOfAreas(polygons) / polygons.size();
  }
  double maxArea(const std::vector< nikitin::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), polygonArea);
    return *std::max_element(areas.cbegin(), areas.cend());
  }
  void printMaxPolygonArea(const std::vector< nikitin::Polygon >& polygon, std::ostream& out)
  {
    nikitin::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << maxArea(polygon);
  }
  double minArea(const std::vector< nikitin::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), polygonArea);
    return *std::min_element(areas.cbegin(), areas.cend());
  }
  void printMinPolygonArea(const std::vector< nikitin::Polygon >& polygon, std::ostream& out)
  {
    nikitin::StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << minArea(polygon);
  }
  std::size_t maxVertex(const std::vector< nikitin::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< std::size_t > verticies;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(verticies), getVerticies);
    return *std::max_element(verticies.cbegin(), verticies.cend());
  }
  void printMaxVerticies(const std::vector< nikitin::Polygon >& polygons, std::ostream& out)
  {
    nikitin::StreamGuard guard(out);
    out << maxVertex(polygons);
  }
  std::size_t minVertex(const std::vector< nikitin::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    std::vector< std::size_t > verticies;
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(verticies), getVerticies);
    return *std::min_element(verticies.cbegin(), verticies.cend());
  }
  void printMinVerticies(const std::vector< nikitin::Polygon >& polygons, std::ostream& out)
  {
    nikitin::StreamGuard guard(out);
    out << minVertex(polygons);
  }
  std::size_t evenCount(const std::vector< nikitin::Polygon >& polygons)
  {
    return std::count_if(polygons.cbegin(), polygons.cend(), evenVerticies);
  }
  std::size_t oddCount(const std::vector< nikitin::Polygon >& polygons)
  {
    return std::count_if(polygons.cbegin(), polygons.cend(), oddVerticies);
  }
  std::size_t verticiesCount(const std::vector< nikitin::Polygon >& polygons, std::size_t n)
  {
    auto functor = std::bind(equalVerticies, n, std::placeholders::_1);
    return std::count_if(polygons.cbegin(), polygons.cend(), functor);
  }
  bool compareXPoint(const nikitin::Point& p1, const nikitin::Point& p2)
  {
    return p1.x < p2.x;
  }
  bool compareYPoint(const nikitin::Point& p1, const nikitin::Point& p2)
  {
    return p1.y < p2.y;
  }
  int findMaxXPolygon(const nikitin::Polygon& polygon)
  {
    return (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), compareXPoint)).x;
  }
  int findMaxYPolygon(const nikitin::Polygon& polygon)
  {
    return (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), compareYPoint)).y;
  }
  int findMinXPolygon(const nikitin::Polygon& polygon)
  {
    return (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), compareXPoint)).x;
  }
  int findMinYPolygon(const nikitin::Polygon& polygon)
  {
    return (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), compareYPoint)).y;
  }
  bool compareMaxXPolygon(const nikitin::Polygon& p1, const nikitin::Polygon& p2)
  {
    return findMaxXPolygon(p1) < findMaxXPolygon(p2);
  }
  bool compareMaxYPolygon(const nikitin::Polygon& p1, const nikitin::Polygon& p2)
  {
    return findMaxYPolygon(p1) < findMaxYPolygon(p2);
  }
  bool compareMinXPolygon(const nikitin::Polygon& p1, const nikitin::Polygon& p2)
  {
    return findMinXPolygon(p1) < findMinXPolygon(p2);
  }
  bool compareMinYPolygon(const nikitin::Polygon& p1, const nikitin::Polygon& p2)
  {
    return findMinYPolygon(p1) < findMinYPolygon(p2);
  }
  int findMaxXVector(const std::vector< nikitin::Polygon >& polygons)
  {
    return findMaxXPolygon(*std::max_element(polygons.cbegin(), polygons.cend(), compareMaxXPolygon));
  }
  int findMaxYVector(const std::vector< nikitin::Polygon >& polygons)
  {
    return findMaxYPolygon(*std::max_element(polygons.cbegin(), polygons.cend(), compareMaxYPolygon));
  }
  int findMinXVector(const std::vector< nikitin::Polygon >& polygons)
  {
    return findMinXPolygon(*std::min_element(polygons.cbegin(), polygons.cend(), compareMinXPolygon));
  }
  int findMinYVector(const std::vector< nikitin::Polygon >& polygons)
  {
    return findMinYPolygon(*std::min_element(polygons.cbegin(), polygons.cend(), compareMinYPolygon));
  }
  void outputInframeResult(std::ostream& out, bool result)
  {
    if (result != false)
    {
      out << "<TRUE>";
      return;
    }
    out << "<FALSE>";
  }
  nikitin::Point getSide(const nikitin::Point& p1, const nikitin::Point& p2)
  {
    return {p1.x - p2.x, p1.y - p2.y};
  }
  bool isRightAngle(const nikitin::Point& p1, const nikitin::Point& p2)
  {
    return (p1.x * p2.x + p1.y * p2.y) == 0;
  }
  bool haveRightAngle(const nikitin::Polygon& polygon)
  {
    std::vector< nikitin::Point > sides;
    auto bIt = polygon.points.begin();
    auto eIt = polygon.points.end();
    std::transform(bIt + 1, eIt, bIt, std::back_inserter(sides), getSide);
    sides.push_back(getSide(polygon.points.front(), polygon.points.back()));
    sides.push_back(sides.front());
    std::vector< bool > isRghtShapes;
    std::transform(sides.begin() + 1, sides.end(), sides.begin(), std::back_inserter(isRghtShapes), isRightAngle);
    return std::accumulate(isRghtShapes.cbegin(), isRghtShapes.cend(), 0) != 0;
  }
}

void nikitin::areaPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< double() > > subcommands;
  subcommands["EVEN"] = std::bind(evenArea, std::cref(polygons));
  subcommands["ODD"] = std::bind(oddArea, std::cref(polygons));
  subcommands["MEAN"] = std::bind(meanArea, std::cref(polygons));

  std::string subcommand;
  in >> subcommand;
  double result = 0.0;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    std::size_t verticies = std::stoull(subcommand);
    if (verticies < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    result = verticiesArea(polygons, verticies);
  }
  nikitin::StreamGuard guard(out);
  out << std::fixed << std::setprecision(1) << result;
}

void nikitin::maxPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::map< std::string, std::function< void(std::ostream &) > > subcommands;
  subcommands["AREA"] = std::bind(printMaxPolygonArea, std::cref(polygons), std::placeholders::_1);
  subcommands["VERTEXES"] = std::bind(printMaxVerticies, std::cref(polygons), std::placeholders::_1);

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}

void nikitin::minPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  std::map< std::string, std::function< void(std::ostream &) > > subcommands;
  subcommands["AREA"] = std::bind(printMinPolygonArea, std::cref(polygons), std::placeholders::_1);
  subcommands["VERTEXES"] = std::bind(printMinVerticies, std::cref(polygons), std::placeholders::_1);

  std::string subcommand;
  in >> subcommand;
  subcommands.at(subcommand)(out);
}

void nikitin::countPrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::map< std::string, std::function< std::size_t() > > subcommands;
  subcommands["EVEN"] = std::bind(evenCount, std::cref(polygons));
  subcommands["ODD"] = std::bind(oddCount, std::cref(polygons));

  std::string subcommand;
  in >> subcommand;
  std::size_t result = 0;
  try
  {
    result = subcommands.at(subcommand)();
  }
  catch (...)
  {
    std::size_t verticies = std::stoull(subcommand);
    if (verticies < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    result = verticiesCount(polygons, verticies);
  }
  nikitin::StreamGuard guard(out);
  out << result;
}

void nikitin::inframePrint(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon temp;
  in >> temp;
  if (in.fail() || polygons.empty())
  {
    throw std::invalid_argument("<INVALID COMMAND>");
  }
  int max_temp_x = findMaxXPolygon(temp);
  int max_temp_y = findMaxYPolygon(temp);
  int min_temp_x = findMinXPolygon(temp);
  int min_temp_y = findMinYPolygon(temp);
  int max_x = findMaxXVector(polygons);
  int max_y = findMaxYVector(polygons);
  int min_x = findMinXVector(polygons);
  int min_y = findMinYVector(polygons);
  bool for_max = max_temp_x <= max_x && max_temp_y <= max_y;
  bool for_min = min_temp_x >= min_x && min_temp_y >= min_y;
  bool result = for_max && for_min;
  outputInframeResult(out, result);
}

void nikitin::rightshapesPrint(std::ostream& out, const std::vector< Polygon >& polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), haveRightAngle);
}
