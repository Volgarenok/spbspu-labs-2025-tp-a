#include "Commands.h"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <stdexcept>
#include <vector>
#include <Guard.h>
#include "Polygon.h"

namespace
{
  bool isEven(const mezentsev::Polygon& poly)
  {
    return poly.points.size() % 2 == 0;
  }

  bool isOdd(const mezentsev::Polygon& poly)
  {
    return !isEven(poly);
  }

  bool isAll(const mezentsev::Polygon&)
  {
    return true;
  }

  struct VertexPred
  {
    size_t count;
    bool operator()(const mezentsev::Polygon& poly)
    {
      return poly.points.size() == count;
    }
  };

  template< typename Pred >
  double arSum(const std::vector< mezentsev::Polygon >& polygons, Pred p)
  {
    std::vector< mezentsev::Polygon > filted;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filted), p);
    std::vector< double > areas;
    std::transform(filted.begin(), filted.end(), std::back_inserter(areas), mezentsev::getArea);
    return std::accumulate(areas.begin(), areas.end(), 0.0);
  }

  double arOdd(const std::vector< mezentsev::Polygon >& polygons)
  {
    return arSum(polygons, isOdd);
  }

  double arMean(const std::vector< mezentsev::Polygon >& polygons)
  {
    if (polygons.empty())
    {
      throw std::logic_error("It is not polygons");
    }
    return arSum(polygons, isAll) / polygons.size();
  }

  double arEven(const std::vector< mezentsev::Polygon >& polygons)
  {
    return arSum(polygons, isEven);
  }

  double arNum(const std::vector< mezentsev::Polygon >& polygons, size_t n)
  {
    return arSum(polygons, VertexPred{ n });
  }

  bool compareVert(const mezentsev::Polygon& poly1, const mezentsev::Polygon& poly2)
  {
    return poly1.points.size() < poly2.points.size();
  }

  bool compareAr(const mezentsev::Polygon& poly1, const mezentsev::Polygon& poly2)
  {
    return mezentsev::getArea(poly1) < mezentsev::getArea(poly2);
  }

  void maxAr(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareAr));
    mezentsev::Guard scope(out);
    out << std::fixed << std::setprecision(1) << mezentsev::getArea(max) << "\n";
  }

  void minAr(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareAr));
    mezentsev::Guard scope(out);
    out << std::fixed << std::setprecision(1) << mezentsev::getArea(min) << "\n";
  }

  void maxVert(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    auto max = (*std::max_element(polygons.begin(), polygons.end(), compareVert));
    out << max.points.size() << "\n";
  }

  void minVert(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    auto min = (*std::min_element(polygons.begin(), polygons.end(), compareVert));
    out << min.points.size() << "\n";
  }

  template< typename Pred >
  size_t countIf(const std::vector< mezentsev::Polygon >& polygons, Pred p)
  {
    std::vector< mezentsev::Polygon > filted;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filted), p);
    return filted.size();
  }

  void countOdd(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isOdd) << "\n";
  }

  void countEven(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out)
  {
    out << countIf(polygons, isEven) << "\n";
  }

  void countNum(const std::vector< mezentsev::Polygon >& polygons, std::ostream& out, size_t n)
  {
    out << countIf(polygons, VertexPred{ n }) << "\n";
  }
}

void mezentsev::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string sub;
  in >> sub;
  double re = 0.0;
  std::map< std::string, std::function< double() > > subs;
  subs["EVEN"] = std::bind(arEven, std::cref(polygons));
  subs["ODD"] = std::bind(arOdd, std::cref(polygons));
  subs["MEAN"] = std::bind(arMean, std::cref(polygons));
  try
  {
    re = subs.at(sub)();
  }
  catch (const std::exception&)
  {
    size_t n = std::stoull(sub);
    if (n < 3)
    {
      throw std::logic_error("Few peaks");
    }
    re = arNum(polygons, n);
  }
  mezentsev::Guard scope(out);
  out << std::fixed << std::setprecision(1) << re << "\n";
}

void mezentsev::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string sub;
  in >> sub;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subs;
  subs["AREA"] = std::bind(minAr, std::cref(polygons), std::ref(out));
  subs["VERTEXES"] = std::bind(minVert, std::cref(polygons), std::ref(out));
  try
  {
    subs.at(sub)();
  }
  catch (const std::exception&)
  {
    throw std::logic_error("Unknown that command");
  }
}

void mezentsev::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string sub;
  in >> sub;
  std::map< std::string, std::function< void() > > subs;
  subs["EVEN"] = std::bind(countEven, std::cref(polygons), std::ref(out));
  subs["ODD"] = std::bind(countOdd, std::cref(polygons), std::ref(out));
  try
  {
    subs.at(sub)();
  }
  catch (const std::exception&)
  {
    size_t n = std::stoull(sub);
    if (n < 3)
    {
      throw std::logic_error("Few peaks");
    }
    countNum(polygons, out, n);
  }
}

void mezentsev::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string sub;
  in >> sub;
  if (polygons.empty())
  {
    throw std::logic_error("No polygons");
  }
  std::map< std::string, std::function< void() > > subs;
  subs["AREA"] = std::bind(maxAr, std::cref(polygons), std::ref(out));
  subs["VERTEXES"] = std::bind(maxVert, std::cref(polygons), std::ref(out));
  try
  {
    subs.at(sub)();
  }
  catch (const std::exception&)
  {
    throw std::logic_error("Unknown that command");
  }
}

void mezentsev::lessArea(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon poly;
  in >> poly;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALIND COMMAND>");
  }
  using namespace std::placeholders;
  out << std::count_if(polygons.begin(), polygons.end(), std::bind(compareAr, _1, poly)) << "\n";
}

void mezentsev::maxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon poly;
  in >> poly;
  if (!in || in.peek() != '\n')
  {
    in.clear();
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t maxCount = 0;
  size_t currentCount = 0;
  for (const auto& p : polygons)
  {
    if (p == poly)
    {
      ++currentCount;
      maxCount = std::max(maxCount, currentCount);
    }
    else
    {
      currentCount = 0;
    }
  }
  out << maxCount << "\n";
}
