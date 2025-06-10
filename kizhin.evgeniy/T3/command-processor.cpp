#include "command-processor.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <stream-guard.hpp>

namespace kizhin {
  using CmdContainer = std::map< std::string, std::function< void(void) > >;
  double computeArea(const Polygon&);

  void processArea(const PolygonContainer&, std::istream&, std::ostream&);
  void processMax(const PolygonContainer&, std::istream&, std::ostream&);
  void processMin(const PolygonContainer&, std::istream&, std::ostream&);
  void processCount(const PolygonContainer&, std::istream&, std::ostream&);
  void processPerms(const PolygonContainer&, std::istream&, std::ostream&);
  void processMaxseq(const PolygonContainer&, std::istream&, std::ostream&);

  namespace area {
    void processEven(const PolygonContainer&, std::ostream&);
    void processOdd(const PolygonContainer&, std::ostream&);
    void processMean(const PolygonContainer&, std::ostream&);
    void processVertexCount(const PolygonContainer&, std::size_t, std::ostream&);
  }

  namespace max {
    void processArea(const PolygonContainer&, std::ostream&);
    void processVertexes(const PolygonContainer&, std::ostream&);
  }

  namespace min {
    void processArea(const PolygonContainer&, std::ostream&);
    void processVertexes(const PolygonContainer&, std::ostream&);
  }

  namespace count {
    void processEven(const PolygonContainer&, std::ostream&);
    void processOdd(const PolygonContainer&, std::ostream&);
    void processVertexCount(const PolygonContainer&, std::size_t, std::ostream&);
  }

  using namespace std::placeholders;
  const auto pointsSize = std::mem_fn(&PointContainer::size);
  const auto polygonSize = std::bind(pointsSize, std::bind(&Polygon::points, _1));
  const auto polygonArea = std::bind(computeArea, _1);

  const auto modTwo = std::bind(std::modulus<>{}, std::bind(polygonSize, _1), 2);
  const auto isEven = std::bind(std::equal_to<>{}, std::bind(modTwo, _1), 0);
  const auto isOdd = std::bind(std::logical_not<>{}, std::bind(isEven, _1));

  const auto areaCompPart = std::bind(std::less<>{}, std::bind(polygonArea, _1), _2);
  const auto areaComp = std::bind(areaCompPart, _1, std::bind(polygonArea, _2));

  const auto vertexCompPart = std::bind(std::less<>{}, std::bind(polygonSize, _1), _2);
  const auto vertexComp = std::bind(vertexCompPart, _1, std::bind(polygonSize, _2));
}

void kizhin::processCommands(PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  const StreamGuard guard(out);
  const auto inRef = std::ref(in);
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "AREA", std::bind(processArea, std::cref(polygons), inRef, outRef) },
    { "MAX", std::bind(processMax, std::cref(polygons), inRef, outRef) },
    { "MIN", std::bind(processMin, std::cref(polygons), inRef, outRef) },
    { "COUNT", std::bind(processCount, std::cref(polygons), inRef, outRef) },
    { "PERMS", std::bind(processPerms, std::cref(polygons), inRef, outRef) },
    { "MAXSEQ", std::bind(processMaxseq, std::cref(polygons), inRef, outRef) },
  };
  out << std::fixed << std::setprecision(1);
  CmdContainer::key_type currCmd;
  while (in >> currCmd) {
    try {
      commands.at(currCmd)();
    } catch (const std::logic_error&) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}

void kizhin::processArea(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "EVEN", std::bind(area::processEven, std::cref(polygons), outRef) },
    { "ODD", std::bind(area::processOdd, std::cref(polygons), outRef) },
    { "MEAN", std::bind(area::processMean, std::cref(polygons), outRef) },
  };
  std::size_t vertexCount = 0;
  if (in >> vertexCount) {
    area::processVertexCount(polygons, vertexCount, out);
    return;
  }
  in.clear();
  CmdContainer::key_type currCmd;
  if (!(in >> currCmd)) {
    throw std::logic_error("Failed to input command");
  }
  commands.at(currCmd)();
}

void kizhin::processMax(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "AREA", std::bind(max::processArea, std::cref(polygons), outRef) },
    { "VERTEXES", std::bind(max::processVertexes, std::cref(polygons), outRef) },
  };
  CmdContainer::key_type currCmd;
  if (!(in >> currCmd)) {
    throw std::logic_error("Failed to input command");
  }
  commands.at(currCmd)();
}

void kizhin::processMin(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "AREA", std::bind(min::processArea, std::cref(polygons), outRef) },
    { "VERTEXES", std::bind(min::processVertexes, std::cref(polygons), outRef) },
  };
  CmdContainer::key_type currCmd;
  if (!(in >> currCmd)) {
    throw std::logic_error("Failed to input command");
  }
  commands.at(currCmd)();
}

void kizhin::processCount(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  const auto outRef = std::ref(out);
  const CmdContainer commands = {
    { "EVEN", std::bind(count::processEven, std::cref(polygons), outRef) },
    { "ODD", std::bind(count::processOdd, std::cref(polygons), outRef) },
  };
  std::size_t vertexCount = 0;
  if (in >> vertexCount) {
    count::processVertexCount(polygons, vertexCount, out);
    return;
  }
  in.clear();
  CmdContainer::key_type currCmd;
  if (!(in >> currCmd)) {
    throw std::logic_error("Failed to input command");
  }
  commands.at(currCmd)();
}

void kizhin::processPerms(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  Polygon target;
  if (!(in >> target) || polygons.empty()) {
    throw std::logic_error("Failed to input polygon or empty polygons");
  }
  using PntIt = PointContainer::const_iterator;
  PntIt (PointContainer::*beg)() const = &PointContainer::begin;
  PntIt (PointContainer::*end)() const = &PointContainer::end;
  const auto getPoints = std::bind(&Polygon::points, _1);
  const auto getBegin = std::bind(beg, std::bind(getPoints, _1));
  const auto getEnd = std::bind(end, std::bind(getPoints, _1));
  const PointContainer& targPnts = target.points;
  bool (*isPerm)(PntIt, PntIt, PntIt, PntIt) = std::is_permutation< PntIt, PntIt >;
  const auto isPermutation = std::bind(isPerm, std::bind(getBegin, _1),
      std::bind(getEnd, _1), targPnts.begin(), targPnts.end());
  out << std::count_if(polygons.begin(), polygons.end(), isPermutation) << '\n';
}

void kizhin::processMaxseq(const PolygonContainer& polygons, std::istream& in,
    std::ostream& out)
{
  Polygon target;
  if (!(in >> target) || polygons.empty()) {
    throw std::logic_error("Failed to input polygon or empty polygons");
  }
  const std::size_t count = std::count(polygons.begin(), polygons.end(), target);
  if (count == 0) {
    out << 0 << '\n';
  }
  std::vector< std::size_t > sizes(count);
  std::iota(sizes.rbegin(), sizes.rend(), 0);
  using PolygonIt = PolygonContainer::const_iterator;
  const auto searchN = std::search_n< PolygonIt, std::size_t, Polygon >;
  const auto findTarg = std::bind(searchN, polygons.begin(), polygons.end(), _1, target);
  const auto notEq = std::not_equal_to<>{};
  const auto pred = std::bind(notEq, std::bind(findTarg, _1), polygons.end());
  auto res = std::find_if(sizes.begin(), sizes.end(), pred);
  out << *res << '\n';
}

void kizhin::area::processEven(const PolygonContainer& polygons, std::ostream& out)
{
  PolygonContainer copy;
  copy.reserve(std::count_if(polygons.begin(), polygons.end(), isEven));
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(copy), isEven);
  const auto summator = std::bind(std::plus<>{}, _1, std::bind(polygonArea, _2));
  out << std::accumulate(copy.begin(), copy.end(), 0.0, summator) << '\n';
}

void kizhin::area::processOdd(const PolygonContainer& polygons, std::ostream& out)
{
  PolygonContainer copy;
  copy.reserve(std::count_if(polygons.begin(), polygons.end(), isOdd));
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(copy), isOdd);
  const auto summator = std::bind(std::plus<>{}, _1, std::bind(polygonArea, _2));
  out << std::accumulate(copy.begin(), copy.end(), 0.0, summator) << '\n';
}

void kizhin::area::processMean(const PolygonContainer& polygons, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::logic_error("Empty polygons in AREA MEAN");
  }
  const auto summator = std::bind(std::plus<>{}, _1, std::bind(polygonArea, _2));
  const double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, summator);
  out << sum / polygons.size() << '\n';
}

void kizhin::area::processVertexCount(const PolygonContainer& polygons, std::size_t count,
    std::ostream& out)
{
  if (count < 3) {
    throw std::logic_error("Invalid number of vertexes");
  }
  const auto equalTo = std::equal_to<>{};
  const auto isRightSize = std::bind(equalTo, std::bind(polygonSize, _1), count);
  PolygonContainer copy;
  copy.reserve(std::count_if(polygons.begin(), polygons.end(), isRightSize));
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(copy), isRightSize);
  const auto summator = std::bind(std::plus<>{}, _1, std::bind(polygonArea, _2));
  out << std::accumulate(copy.begin(), copy.end(), 0.0, summator) << '\n';
}

void kizhin::max::processArea(const PolygonContainer& polygons, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::logic_error("Empty polygons in MAX AREA");
  }
  const auto res = std::max_element(polygons.begin(), polygons.end(), areaComp);
  out << computeArea(*res) << '\n';
}

void kizhin::max::processVertexes(const PolygonContainer& polygons, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::logic_error("Empty polygons in MAX VERTEXES");
  }
  const auto res = std::max_element(polygons.begin(), polygons.end(), vertexComp);
  out << res->points.size() << '\n';
}

void kizhin::min::processArea(const PolygonContainer& polygons, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::logic_error("Empty polygons in MIN AREA");
  }
  const auto res = std::min_element(polygons.begin(), polygons.end(), areaComp);
  out << computeArea(*res) << '\n';
}

void kizhin::min::processVertexes(const PolygonContainer& polygons, std::ostream& out)
{
  if (polygons.empty()) {
    throw std::logic_error("Empty polygons in MIN VERTEXES");
  }
  const auto res = std::min_element(polygons.begin(), polygons.end(), vertexComp);
  out << res->points.size() << '\n';
}

void kizhin::count::processEven(const PolygonContainer& polygons, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), isEven) << '\n';
}

void kizhin::count::processOdd(const PolygonContainer& polygons, std::ostream& out)
{
  out << std::count_if(polygons.begin(), polygons.end(), isOdd) << '\n';
}

void kizhin::count::processVertexCount(const PolygonContainer& polygons,
    std::size_t count, std::ostream& out)
{
  if (count < 3) {
    throw std::logic_error("Invalid number of vertexes");
  }
  const auto equalTo = std::equal_to<>{};
  const auto isRightSize = std::bind(equalTo, count, std::bind(polygonSize, _1));
  out << std::count_if(polygons.begin(), polygons.end(), isRightSize) << '\n';
}

double kizhin::computeArea(const Polygon& polygon)
{
  if (polygon.points.size() < 3) {
    return 0.0;
  }
  const auto mul = std::multiplies<>{};
  const auto sub = std::minus<>{};
  const auto part = std::bind(mul, std::bind(&Point::x, _1), std::bind(&Point::y, _2));
  const auto det = std::bind(sub, std::bind(part, _1, _2), std::bind(part, _2, _1));

  const auto begin = polygon.points.begin();
  const auto end = polygon.points.end();
  double area = std::inner_product(begin, end - 1, begin + 1, 0.0, std::plus<>{}, det);
  area += det(polygon.points.back(), polygon.points.front());
  return std::abs(area) / 2.0;
}

