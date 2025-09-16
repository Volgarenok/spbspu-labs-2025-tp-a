#include "polygon.hpp"
#include "polygon_iterator.hpp"

namespace ivanova
{
  PolygonIterator::PolygonIterator() : in_(nullptr), end_(true) {}

  PolygonIterator::PolygonIterator(std::istream& in) : in_(&in), end_(false)
  {
    ++(*this);
  }

  PolygonIterator::reference PolygonIterator::operator*() const { return poly_; }
  PolygonIterator::pointer PolygonIterator::operator->() const { return &poly_; }

  bool PolygonIterator::parseCoordinate(const std::string& line, std::size_t& pos, char delimiter, int& value)
  {
    std::size_t start = pos;
    while (pos < line.size() && line[pos] != delimiter) ++pos;
    if (pos == line.size()) return false;
    try {
      value = std::stoi(line.substr(start, pos - start));
    } catch (...) {
      return false;
    }
    ++pos;
    return true;
  }

  PolygonIterator& PolygonIterator::operator++()
  {
    std::string line;
    while (in_ && std::getline(*in_, line))
    {
      std::size_t pos = 0;
      std::size_t count = 0;
      try {
        count = std::stoull(line, &pos);
      } catch (...) {
        continue;
      }
      if (count < 3) continue;

      std::vector<Point> points;
      points.reserve(count);
      bool ok = true;
      int x, y;
      for (std::size_t i = 0; i < count; ++i) {
        while (pos < line.size() && line[pos] != '(') ++pos;
        if (pos == line.size()) { ok = false; break; }
        ++pos;
        if (!parseCoordinate(line, pos, ';', x)) { ok = false; break; }
        if (!parseCoordinate(line, pos, ')', y)) { ok = false; break; }
        points.push_back(Point{x, y});
      }

      while (pos < line.size() && std::isspace(line[pos])) ++pos;

      if (ok && points.size() == count && pos == line.size()) {
        poly_.points = std::move(points);
        return *this;
      }
    }
    end_ = true;
    in_ = nullptr;
    return *this;
  }

  PolygonIterator PolygonIterator::operator++(int)
  {
    PolygonIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool PolygonIterator::operator==(const PolygonIterator& other) const
  {
    return (end_ && other.end_) || (in_ == other.in_);
  }

  bool PolygonIterator::operator!=(const PolygonIterator& other) const
  {
    return !(*this == other);
  }
}
