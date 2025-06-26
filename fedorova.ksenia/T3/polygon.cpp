#include "polygon.h"
#include <algorithm>
#include <numeric>
#include <cmath>

bool fedorova::operator==(const Point& lhs, const Point& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool fedorova::operator==(const Polygon& lhs, const Polygon& rhs)
{
    return lhs.points == rhs.points;
}

std::istream& fedorova::operator>>(std::istream& in, Point& p)
{
    char discard;
    if (!(in >> discard >> p.x >> discard >> p.y >> discard) || discard != ')')
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& fedorova::operator>>(std::istream& in, Polygon& poly)
{
    size_t vertexCount;
    if (!(in >> vertexCount) || vertexCount < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    poly.points.resize(vertexCount);
    for (auto& point : poly.points)
    {
        if (!(in >> point))
        {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    return in;
}

double fedorova::getArea(const Polygon& poly)
{
    double area = 0.0;
    const auto& points = poly.points;
    size_t n = points.size();

    for (size_t i = 0; i < n; ++i)
    {
        size_t j = (i + 1) % n;
        area += (points[i].x * points[j].y) - (points[j].x * points[i].y);
    }

    return std::abs(area) / 2.0;
}

bool fedorova::hasRightAngle(const Polygon& poly)
{
    const auto& points = poly.points;
    size_t n = points.size();

    for (size_t i = 0; i < n; ++i)
    {
        size_t prev = (i + n - 1) % n;
        size_t next = (i + 1) % n;

        int abx = points[i].x - points[prev].x;
        int aby = points[i].y - points[prev].y;
        int bcx = points[next].x - points[i].x;
        int bcy = points[next].y - points[i].y;

        int dotProduct = abx * bcx + aby * bcy;
        if (dotProduct == 0)
        {
            return true;
        }
    }
    return false;
}

bool fedorova::isPermutation(const Polygon& a, const Polygon& b)
{
    if (a.points.size() != b.points.size())
    {
        return false;
    }

    auto aPoints = a.points;
    auto bPoints = b.points;

    std::sort(aPoints.begin(), aPoints.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
        });

    std::sort(bPoints.begin(), bPoints.end(), [](const Point& p1, const Point& p2) {
        return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
        });

    return aPoints == bPoints;
}
