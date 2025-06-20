#ifndef POLYGON_ITERATOR_HPP
#define POLYGON_ITERATOR_HPP
#include <istream>
#include <string>
#include "polygon.hpp"

namespace ivanova
{
  class PolygonIterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = Polygon;
    using difference_type = std::ptrdiff_t;
    using pointer = const Polygon*;
    using reference = const Polygon&;

    PolygonIterator();
    explicit PolygonIterator(std::istream& in);

    reference operator*() const;
    pointer operator->() const;

    PolygonIterator& operator++();
    PolygonIterator operator++(int);

    bool operator==(const PolygonIterator& other) const;
    bool operator!=(const PolygonIterator& other) const;

  private:
    std::istream* in_;
    Polygon poly_;
    bool end_;

    bool parseCoordinate(const std::string& line, std::size_t& pos, char delimiter, int& value);
  };
}

#endif
