#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <utility>

#include "commands.hpp"
#include "polygon.hpp"
#include "streamGuard.hpp"

namespace ivanova
{
  void areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
  {
    std::string param;
    in >> param;

    double answer = 0.0;
    if (param == "EVEN" || param == "ODD" || param == "MEAN")
    {
      answer = area(data, param);
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), isDigit))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      std::size_t vertices = std::stoull(param);
      if (vertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      answer = area(data, vertices);
    }
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1) << answer << "\n";
  }

  void maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
    if (param == "AREA")
    {
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << maxArea(src) << "\n";
    }
    else if (param == "VERTEXES")
    {
      out << maxVertexes(src) << "\n";
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
    if (param == "AREA")
    {
      StreamGuard guard(out);
      out << std::fixed << std::setprecision(1) << minArea(src) << "\n";
    }
    else if (param == "VERTEXES")
    {
      out << minVertexes(src) << "\n";
    }
    else
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
  }

  void countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    std::string param;
    in >> param;
  
    std::size_t answer = 0;
    if (param == "EVEN" || param == "ODD")
    {
      answer = count(src, param);
    }
    else
    {
      if (!std::all_of(param.begin(), param.end(), isDigit))
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      std::size_t vertices = std::stoull(param);
      if (vertices < 3)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
      answer = count(src, vertices);
    }
    out << answer << "\n";
  }

  void echoCommand(std::istream& in, std::ostream& out, std::vector< Polygon >& src)
  {
    Polygon target;
    if (!(in >> target) || target.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << echo(src, target) << "\n";
  }

  void sameCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& src)
  {
    Polygon target;
    if (!(in >> target) || target.size() < 3)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }
    out << same(src, target) << "\n";
  }

  void printCommand(std::ostream& out, const std::vector< Polygon>& src)
  {
    std::copy(src.begin(), src.end(), std::ostream_iterator< Polygon >(out, "\n"));
  }
}
