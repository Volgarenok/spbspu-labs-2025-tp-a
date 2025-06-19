#include "polygon_iterator.hpp"
#include <sstream>

namespace ivanova
{
  PolygonIterator::PolygonIterator() : in_(nullptr), end_(true) {}

  PolygonIterator::PolygonIterator(std::istream& in) : in_(&in), end_(false)
  {
    ++(*this);
  }

  PolygonIterator::reference PolygonIterator::operator*() const { return poly_; }
  PolygonIterator::pointer PolygonIterator::operator->() const { return &poly_; }

  PolygonIterator& PolygonIterator::operator++()
  {
    std::string line;
    while (in_ && std::getline(*in_, line))
    {
      std::istringstream iss(line);
      Polygon temp;
      if (iss >> temp)
      {
        char c;
        if (!(iss >> c))
        {
          poly_ = std::move(temp);
          return *this;
        }
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
