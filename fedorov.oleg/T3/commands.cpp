#include "commands.hpp"
#include <algorithm>
#include <functional>
#include <map>
#include <stdexcept>
#include <iomanip>
#include <numeric>
#include <iterator>

#include <format_guard.hpp>
#include "functional.hpp"

namespace fedorov
{
  namespace
  {
    using namespace std::placeholders;

    auto getArea = std::bind(&calcPolygonArea, _1);
    auto getVertexCount = std::bind(&std::vector< Point >::size, std::bind(&Polygon::points, _1));

    auto modTwo = std::bind(std::modulus< size_t >{}, std::bind(getVertexCount, _1), 2);
    auto isOdd = std::bind(std::not_equal_to< size_t >{}, std::bind(modTwo, _1), 0);
    auto isEven = std::bind(std::equal_to< size_t >{}, std::bind(modTwo, _1), 0);

    auto areaLess = std::bind(std::less< double >{}, std::bind(getArea, _1), std::bind(getArea, _2));

    auto vertexLess = std::bind(std::less< size_t >{}, std::bind(getVertexCount, _1), std::bind(getVertexCount, _2));

    auto areaSummator = std::bind(std::plus< double >{}, _1, std::bind(getArea, _2));

    void outputArea(double value, std::ostream &out)
    {
      FormatGuard guard(out);
      out << std::fixed << std::setprecision(1) << value << '\n';
    }
  }

  void area::processEven(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEven);
    double total = std::accumulate(filtered.begin(), filtered.end(), 0.0, areaSummator);
    outputArea(total, out);
  }

  void area::processOdd(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOdd);
    double total = std::accumulate(filtered.begin(), filtered.end(), 0.0, areaSummator);
    outputArea(total, out);
  }

  void area::processMean(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("No polygons");
    }
    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaSummator);
    outputArea(total / polygons.size(), out);
  }

  void area::processVertexCount(const std::vector< Polygon > &polygons, size_t num, std::ostream &out)
  {
    if (num < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    auto vertexEqual = std::bind(std::equal_to< size_t >{}, std::bind(getVertexCount, _1), num);
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), vertexEqual);
    double total = std::accumulate(filtered.begin(), filtered.end(), 0.0, areaSummator);
    outputArea(total, out);
  }

  void max::processArea(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty polygons");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    outputArea(calcPolygonArea(*it), out);
  }

  void max::processVertexes(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty polygons");
    }
    auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << '\n';
  }

  void min::processArea(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty polygons");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    outputArea(calcPolygonArea(*it), out);
  }

  void min::processVertexes(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    if (polygons.empty())
    {
      throw std::logic_error("Empty polygons");
    }
    auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << '\n';
  }

  void count::processEven(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << '\n';
  }

  void count::processOdd(const std::vector< Polygon > &polygons, std::ostream &out)
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << '\n';
  }

  void count::processVertexCount(const std::vector< Polygon > &polygons, size_t num, std::ostream &out)
  {
    if (num < 3)
    {
      throw std::invalid_argument("Invalid vertex count");
    }
    auto vertexEqual = std::bind(std::equal_to< size_t >{}, std::bind(getVertexCount, _1), num);
    out << std::count_if(polygons.begin(), polygons.end(), vertexEqual) << '\n';
  }

  void areaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param))
    {
      throw std::invalid_argument("Invalid command");
    }

    using Handler = void (*)(const std::vector< Polygon > &, std::ostream &);
    std::map< std::string, Handler > handlers = {
        {"ODD", area::processOdd}, {"EVEN", area::processEven}, {"MEAN", area::processMean}};

    auto it = handlers.find(param);
    if (it != handlers.end())
    {
      it->second(polygons, out);
    }
    else
    {
      size_t num = std::stoul(param);
      area::processVertexCount(polygons, num, out);
    }
  }

  void maxCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      throw std::logic_error("Empty polygon list");
    }

    std::string param;
    if (!(in >> param))
    {
      throw std::invalid_argument("Invalid command");
    }

    using Handler = void (*)(const std::vector< Polygon > &, std::ostream &);
    std::map< std::string, Handler > handlers = {{"AREA", max::processArea}, {"VERTEXES", max::processVertexes}};

    auto it = handlers.find(param);
    if (it != handlers.end())
    {
      it->second(polys, out);
    }
    else
    {
      throw std::invalid_argument("Invalid subcommand");
    }
  }

  void minCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polys)
  {
    if (polys.empty())
    {
      throw std::logic_error("Empty polygon list");
    }

    std::string param;
    if (!(in >> param))
    {
      throw std::invalid_argument("Invalid command");
    }

    using Handler = void (*)(const std::vector< Polygon > &, std::ostream &);
    std::map< std::string, Handler > handlers = {{"AREA", min::processArea}, {"VERTEXES", min::processVertexes}};

    auto it = handlers.find(param);
    if (it != handlers.end())
    {
      it->second(polys, out);
    }
    else
    {
      throw std::invalid_argument("Invalid subcommand");
    }
  }

  void countCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    std::string param;
    if (!(in >> param))
    {
      throw std::invalid_argument("Invalid command");
    }

    using Handler = void (*)(const std::vector< Polygon > &, std::ostream &);
    std::map< std::string, Handler > handlers = {{"ODD", count::processOdd}, {"EVEN", count::processEven}};

    auto it = handlers.find(param);
    if (it != handlers.end())
    {
      it->second(polygons, out);
    }
    else
    {
      size_t num = std::stoul(param);
      count::processVertexCount(polygons, num, out);
    }
  }

  void lessAreaCommand(std::istream &in, std::ostream &out, const std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (!(in >> target) || !validatePolygon(target))
    {
      throw std::invalid_argument("Invalid polygon");
    }

    double threshold = calcPolygonArea(target);
    auto calcAreaLess = std::bind(std::less< double >{}, std::bind(calcPolygonArea, _1), threshold);
    out << std::count_if(polygons.begin(), polygons.end(), calcAreaLess) << '\n';
  }

  void echoCommand(std::istream &in, std::ostream &out, std::vector< Polygon > &polygons)
  {
    Polygon target;
    if (!(in >> target) || !validatePolygon(target))
    {
      throw std::invalid_argument("Invalid polygon");
    }

    size_t count = std::count(polygons.begin(), polygons.end(), target);

    std::vector< Polygon > result;
    result.reserve(polygons.size() + count);

    std::copy(polygons.begin(), polygons.end(), std::back_inserter(result));

    auto polygonEqT = std::bind(std::equal_to< Polygon >(), _1, target);
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(result), polygonEqT);

    polygons.swap(result);
    out << count << '\n';
  }
}
