#include "echo.hpp"
#include <algorithm>

std::size_t pilugina::echoInner(std::vector< Polygon > &polys, const Polygon &poly, vecIt it, std::size_t count)
{
  it = std::find(it, polys.end(), poly);
  if (it == polys.end())
  {
    return count;
  }
  it = polys.insert(std::next(it), poly);
  return echoInner(polys, poly, std::next(it), count + 1);
}

std::size_t pilugina::echo(std::vector< Polygon > &polys, const Polygon &poly)
{
  return echoInner(polys, poly, polys.begin(), 0ull);
}
