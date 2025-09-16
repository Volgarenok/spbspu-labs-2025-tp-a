#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <string>
#include <limits>
#include "streamguard.hpp"
#include "utils.hpp"

namespace klimova {

  void areaEvenCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    auto bindEven = std::bind(areaEven, _1, _2);
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, bindEven);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << result << "\n";
  }

  void areaOddCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    auto bindOdd = std::bind(areaOdd, _1, _2);
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, bindOdd);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << result << "\n";
  }

  void areaMeanCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    if (polygons.empty()) {
      throw std::out_of_range("");
    }
    auto bindMean = std::bind(areaMean, _1, _2, polygons.size());
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, bindMean);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << result << "\n";
  }

  void areaNumCmd(const VecPolygon& polygons, const std::string& numStr, std::ostream& os)
  {
    size_t vertexes = getVertexes(numStr);
    if (!isValidVertexCount(vertexes)) {
      throw std::out_of_range("");
    }
    auto bindMean = std::bind(areaNum, _1, _2, vertexes);
    double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, bindMean);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << result << "\n";
  }

  void maxAreaCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    auto bindArea = std::bind(getArea, _1);
    auto bind1 = std::bind(bindArea, _1);
    auto bind2 = std::bind(bindArea, _2);
    auto compare = std::bind(std::less< double >(), bind1, bind2);
    auto it = std::max_element(polygons.begin(), polygons.end(), compare);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
  }

  void maxVertexesCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    auto bindVertexes = std::bind(getVertexesCount, _1);
    auto bind1 = std::bind(bindVertexes, _1);
    auto bind2 = std::bind(bindVertexes, _2);
    auto compare = std::bind(std::less< size_t >(), bind1, bind2);
    auto it = std::max_element(polygons.begin(), polygons.end(), compare);
    Streamguard guard(os);
    os << getVertexesCount(*it) << "\n";
  }

  void minAreaCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
  {
    auto bindArea = std::bind(getArea, _1);
    auto bind1 = std::bind(bindArea, _1);
    auto bind2 = std::bind(bindArea, _2);
    auto compare = std::bind(std::less< double >(), bind1, bind2);
    auto it = std::min_element(polygons.begin(), polygons.end(), compare);
    Streamguard guard(os);
    os << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
  }

 void minVertexesCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
 {
   auto bindVertexes = std::bind(getVertexesCount, _1);
   auto bind1 = std::bind(bindVertexes, _1);
   auto bind2 = std::bind(bindVertexes, _2);
   auto compare = std::bind(std::less< size_t >(), bind1, bind2);
   auto it = std::min_element(polygons.begin(), polygons.end(), compare);
   Streamguard guard(os);
   os << getVertexesCount(*it) << "\n";
 }

 void countEvenCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
 {
   auto bindPredicate = std::bind(isVertexCountEven, _1);
   size_t result = std::count_if(polygons.begin(), polygons.end(), bindPredicate);
   os << result << "\n";
 }

 void countOddCmd(const VecPolygon& polygons, std::istream&, std::ostream& os)
 {
   auto bindPredicate = std::bind(isVertexCountOdd, _1);
   size_t result = std::count_if(polygons.begin(), polygons.end(), bindPredicate);
   os << result << "\n";
  }

 void countNumCmd(const VecPolygon& polygons, const std::string& numStr, std::ostream& os)
 {
   size_t vertexes = getVertexes(numStr);
   if (!isValidVertexCount(vertexes)) {
     throw std::out_of_range("");
   }
   auto bindPredicate = std::bind(hasVertexCount, _1, vertexes);
   size_t result = std::count_if(polygons.begin(), polygons.end(), bindPredicate);
   os << result << "\n";
  }
}

klimova::AreaSubs klimova::createAreaSubs()
{
  return {{"EVEN", areaEvenCmd}, {"ODD", areaOddCmd}, {"MEAN", areaMeanCmd}};
}

klimova::MaxSubs klimova::createMaxSubs()
{
  return {{"AREA", maxAreaCmd}, {"VERTEXES", maxVertexesCmd}};
}

klimova::MinSubs klimova::createMinSubs()
{
  return {{"AREA", minAreaCmd}, {"VERTEXES", minVertexesCmd}};
}

klimova::CountSubs klimova::createCountSubs()
{
  return {{"EVEN", countEvenCmd}, {"ODD", countOddCmd}};
}

void klimova::area(const VecPolygon& polygons, std::istream& is, std::ostream& os)
{
  try {
    AreaSubs subs = createAreaSubs();
    std::string subcommand;
    is >> subcommand;
    auto it = subs.find(subcommand);
    if (it != subs.end()) {
      it->second(polygons, is, os);
    } else {
      areaNumCmd(polygons, subcommand, os);
    }
  }
  catch (const std::exception&) {
    os << "<INVALID COMMAND>\n";
  }
}

void klimova::max(const VecPolygon& polygons, std::istream& is, std::ostream& os)
{
  try {
    if (polygons.empty()) {
      throw std::invalid_argument("");
    }
    MaxSubs subs = createMaxSubs();
    std::string subcommand;
    is >> subcommand;
    auto it = subs.find(subcommand);
    if (it != subs.end()) {
      it->second(polygons, is, os);
    } else {
      throw std::invalid_argument("");
    }
  }
  catch (const std::exception&) {
    os << "<INVALID COMMAND>\n";
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

void klimova::min(const VecPolygon& polygons, std::istream& is, std::ostream& os) {
  try {
    if (polygons.empty()) {
      throw std::invalid_argument("");
    }
    MinSubs subs = createMinSubs();
    std::string subcommand;
    is >> subcommand;
    auto it = subs.find(subcommand);
    if (it != subs.end()) {
      it->second(polygons, is, os);
    } else {
      throw std::invalid_argument("");
    }
  }
  catch (const std::exception&) {
    os << "<INVALID COMMAND>\n";
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

void klimova::count(const VecPolygon& polygons, std::istream& is, std::ostream& os)
{
  try {
    CountSubs subs = createCountSubs();
    std::string subcommand;
    is >> subcommand;
    auto it = subs.find(subcommand);
    if (it != subs.end()) {
      it->second(polygons, is, os);
    } else {
      countNumCmd(polygons, subcommand, os);
    }
  }
  catch (const std::exception&) {
    os << "<INVALID COMMAND>\n";
  }
}

void klimova::perms(const VecPolygon& polygons, std::istream& is, std::ostream& os)
{
  try {
    Polygon target;
    is >> target;
    if (target.points.empty()) {
      throw std::invalid_argument("");
    }

    PermsTester tester{target};
    size_t count = std::count_if(polygons.begin(), polygons.end(), tester);
    os << count << "\n";
  }
  catch (const std::exception&) {
    os << "<INVALID COMMAND>\n";
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

void klimova::rects(const VecPolygon& polygons, std::ostream& os)
{
  RectangleChecker checker;
  size_t count = std::count_if(polygons.begin(), polygons.end(), checker);
  os << count << "\n";
}

klimova::CommandHandler klimova::createCommandHandler(const VecPolygon& polygons)
{
  CommandHandler cmds;
  cmds["AREA"] = std::bind(area, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MAX"] = std::bind(max, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["MIN"] = std::bind(min, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["COUNT"] = std::bind(count, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["PERMS"] = std::bind(perms, std::cref(polygons), std::ref(std::cin), std::ref(std::cout));
  cmds["RECTS"] = std::bind(rects, std::cref(polygons), std::ref(std::cout));
  return cmds;
}
