#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <map>
#include <functional>
#include <numeric>
#include "commands.h"
#include "io-utils.h"

double sherkunov::subArea(const Point& a, const Point& b)
{
    return a.x * b.y - a.y * b.x;
}

double sherkunov::areaPolygon(const Polygon& polygon)
{
    const auto& p = polygon.points;
    double sum = std::inner_product(p.begin(), p.end() - 1, p.begin() + 1, 0.0, std::plus< double >{}, subArea);
    sum += subArea(p.back(), p.front());
    return std::abs(sum) / 2.0;
}

bool sherkunov::isEven(const Polygon& polygon)
{
    return polygon.points.size() % 2 == 0;
}

bool sherkunov::isOdd(const Polygon& polygon)
{
    return polygon.points.size() % 2 != 0;
}

bool sherkunov::isNum(const Polygon& polygon, size_t numOfVertexes)
{
    return polygon.points.size() == numOfVertexes;
}

double sherkunov::oddAreaAccumulator(double sum, const Polygon& polygon)
{
    return isOdd(polygon) ? sum + areaPolygon(polygon) : sum;
}

double sherkunov::evenAreaAccumulator(double sum, const Polygon& polygon)
{
    return isEven(polygon) ? sum + areaPolygon(polygon) : sum;
}

double sherkunov::meanAreaAccumulator(double sum, const Polygon& polygon)
{
    return sum + areaPolygon(polygon);
}

double sherkunov::numAreaAccumulator(double sum, const Polygon& polygon, size_t numOfVertexes)
{
    return isNum(polygon, numOfVertexes) ? sum + areaPolygon(polygon) : sum;
}

double sherkunov::areaEven(const std::vector< Polygon >& polygons)
{
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, evenAreaAccumulator);
}

double sherkunov::areaOdd(const std::vector< Polygon >& polygons)
{
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, oddAreaAccumulator);
}

double sherkunov::areaMean(const std::vector< Polygon >& polygons)
{
    if (polygons.empty()) {
        return 0.0;
    }
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, meanAreaAccumulator) / polygons.size();
}

double sherkunov::areaNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
    using namespace std::placeholders;
    return std::accumulate(polygons.begin(), polygons.end(), 0.0, std::bind(numAreaAccumulator, _1, _2, numOfVertexes));
}

void sherkunov::area(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    std::string subcommand;
    in >> subcommand;

    out << std::fixed << std::setprecision(1);

    if (subcommand == "EVEN")
    {
        out << areaEven(polygons);
    }
     else
  if (subcommand == "ODD")
  {
        out << areaOdd(polygons);
      }
  else if (subcommand == "MEAN")
  {
        if (polygons.empty())
  {
            throw std::logic_error("<INVALID COMMAND>");
        }
  else
  {
            out << areaMean(polygons);
        }
    } else {
        try {
            size_t numOfVertexes = std::stoull(subcommand);
            if (numOfVertexes < 3) {
                throw std::logic_error("<WRONG SUBCOMMAND>");
            }
            out << areaNum(polygons, numOfVertexes);
        } catch (const std::exception&) {
            throw std::logic_error("<WRONG SUBCOMMAND>");
        }
    }
}
void sherkunov::max(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        throw std::logic_error("<THERE ARE NO POLYGONS>");
    }
    std::string subcommand;
    in >> subcommand;

    if (subcommand == "AREA") {
        out << std::fixed << std::setprecision(1) << maxArea(polygons);
    } else if (subcommand == "VERTEXES") {
        out << maxVertexes(polygons); // Без форматирования для целых чисел
    } else {
        throw std::logic_error("<WRONG SUBCOMMAND>");
    }
}

bool sherkunov::areaComparator(const Polygon& a, const Polygon& b)
{
    return areaPolygon(a) < areaPolygon(b);
}

double sherkunov::maxArea(const std::vector< Polygon >& polygons)
{
    return areaPolygon(*std::max_element(polygons.begin(), polygons.end(), areaComparator));
}

bool sherkunov::vertexesComparator(const Polygon& a, const Polygon& b)
{
    return a.points.size() < b.points.size();
}

size_t sherkunov::maxVertexes(const std::vector< Polygon >& polygons)
{
    return (*std::max_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void sherkunov::min(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        throw std::logic_error("<THERE ARE NO POLYGONS>");
    }
    std::string subcommand;
    in >> subcommand;

    if (subcommand == "AREA") {
        out << std::fixed << std::setprecision(1) << minArea(polygons);
    } else if (subcommand == "VERTEXES") {
        out << minVertexes(polygons); // Без форматирования для целых чисел
    } else {
        throw std::logic_error("<WRONG SUBCOMMAND>");
    }
}

double sherkunov::minArea(const std::vector< Polygon >& polygons)
{
    return areaPolygon(*std::min_element(polygons.begin(), polygons.end(), areaComparator));
}

size_t sherkunov::minVertexes(const std::vector< Polygon >& polygons)
{
    return (*std::min_element(polygons.begin(), polygons.end(), vertexesComparator)).points.size();
}

void sherkunov::count(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    std::string subcommand;
    in >> subcommand;

    if (subcommand == "EVEN") {
        out << countEven(polygons);
    } else if (subcommand == "ODD") {
        out << countOdd(polygons);
    } else {
        try {
            size_t numOfVertexes = std::stoull(subcommand);
            if (numOfVertexes < 3) {
                throw std::logic_error("<WRONG SUBCOMMAND>");
            }
            out << countNum(polygons, numOfVertexes);
        } catch (const std::exception&) {
            throw std::logic_error("<WRONG SUBCOMMAND>");
        }
    }
}

size_t sherkunov::countEven(const std::vector< Polygon >& polygons)
{
    return std::count_if(polygons.begin(), polygons.end(), isEven);
}

size_t sherkunov::countOdd(const std::vector< Polygon >& polygons)
{
    return std::count_if(polygons.begin(), polygons.end(), isOdd);
}

size_t sherkunov::countNum(const std::vector< Polygon >& polygons, size_t numOfVertexes)
{
    using namespace std::placeholders;
    return std::count_if(polygons.begin(), polygons.end(), std::bind(isNum, _1, numOfVertexes));
}
void sherkunov::inframe(const std::vector< Polygon >& polygons, std::istream& in, std::ostream& out)
{
    if (polygons.empty())
    {
        throw std::logic_error("<THERE ARE NO POLYGONS>");
    }

    Point point;
    in >> point;

    // Находим границы всех полигонов
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    for (const auto& polygon : polygons)
    {
        for (const auto& p : polygon.points)
        {
            minX = std::min(minX, p.x);
            maxX = std::max(maxX, p.x);
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
    }

    if (point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY)
    {
        out << "<TRUE>";
    }
    else
    {
        out << "<FALSE>";
    }
}
void sherkunov::rightshapes(const std::vector< Polygon >& polygons, std::istream&, std::ostream& out)
{
    size_t count = 0;
    for (const auto& polygon : polygons)
    {
        const auto& points = polygon.points;
        size_t n = points.size();
        bool hasRightAngle = false;

        for (size_t i = 0; i < n && !hasRightAngle; ++i)
        {
            const Point& a = points[i];
            const Point& b = points[(i + 1) % n];
            const Point& c = points[(i + 2) % n];

            int abx = b.x - a.x;
            int aby = b.y - a.y;
            int bcx = c.x - b.x;
            int bcy = c.y - b.y;

            int dotProduct = abx * bcx + aby * bcy;

            if (dotProduct == 0)
            {
                hasRightAngle = true;
            }
        }

        if (hasRightAngle)
        {
            count++;
        }
    }

    out << count;
}
