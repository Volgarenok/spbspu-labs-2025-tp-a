#include "polygon.hpp"
#include "polygon_iterator.hpp"

#include <string>
#include <vector>
#include <cctype>
#include <sstream>

namespace ivanova
{
  PolygonIterator::PolygonIterator():
    in_(nullptr),
    end_(true)
  {}

  PolygonIterator::PolygonIterator(std::istream& in):
    in_(&in),
    end_(false)
  {
    ++(*this);
  }

  PolygonIterator::reference PolygonIterator::operator*() const
  {
    return poly_;
  }

  PolygonIterator::pointer PolygonIterator::operator->() const
  {
    return &poly_;
  }

  struct ParseCoordResult
  {
    bool success;
    int value;
    std::size_t new_pos;
  };

  struct ParsePointsResult
  {
    bool success;
    std::vector< Point > points;
    std::size_t new_pos;
  };

  struct ParseCountResult
  {
    bool success;
    std::size_t count;
    std::size_t new_pos;
  };

  class CoordinateParser
  {
  public:
    ParseCoordResult operator()(const std::string& line, std::size_t pos, char delimiter) const
    {
      if (pos >= line.size())
      {
        return {false, 0, pos};
      }

      std::size_t start = pos;
      std::size_t current = pos;

      if (current < line.size() && line[current] != delimiter)
      {
        auto result = (*this)(line, current + 1, delimiter);
        if (result.success)
        {
          return result;
        }
      }

      if (current < line.size() && line[current] == delimiter)
      {
        try
        {
          int value = std::stoi(line.substr(start, current - start));
          return {true, value, current + 1};
        }
        catch (...)
        {
          return {false, 0, current};
        }
      }

      return {false, 0, pos};
    }
  };

  class PointsParser
  {
  public:
    ParsePointsResult operator()(const std::string& line, std::size_t pos, std::size_t count) const
    {
      return parseRecursive(line, pos, count, std::vector< Point >());
    }

  private:
    ParsePointsResult parseRecursive(const std::string& line, std::size_t pos, std::size_t count, std::vector< Point > accumulated) const
    {
      if (count == 0)
      {
        return {true, accumulated, pos};
      }

      std::size_t current = pos;
      if (current >= line.size() || line[current] != '(')
      {
        if (current < line.size())
        {
          return parseRecursive(line, current + 1, count, accumulated);
        }
        return {false, accumulated, current};
      }

      current++;

      CoordinateParser coord_parser;
      auto x_result = coord_parser(line, current, ';');
      if (!x_result.success)
      {
        return {false, accumulated, x_result.new_pos};
      }

      auto y_result = coord_parser(line, x_result.new_pos, ')');
      if (!y_result.success)
      {
        return {false, accumulated, y_result.new_pos};
      }

      accumulated.push_back(Point{x_result.value, y_result.value});
      return parseRecursive(line, y_result.new_pos, count - 1, accumulated);
    }
  };

  class WhitespaceSkipper
  {
  public:
    std::size_t operator()(const std::string& line, std::size_t pos) const
    {
      return skipRecursive(line, pos);
    }

  private:
    std::size_t skipRecursive(const std::string& line, std::size_t pos) const
    {
      if (pos >= line.size() || !std::isspace(line[pos]))
      {
        return pos;
      }
      return skipRecursive(line, pos + 1);
    }
  };

  class CountParser
  {
  public:
    ParseCountResult operator()(const std::string& line, std::size_t pos = 0) const
    {
      if (pos >= line.size() || !std::isdigit(line[pos]))
      {
        return {false, 0, pos};
      }

      std::size_t start = pos;
      std::size_t current = pos;
      while (current < line.size() && std::isdigit(line[current]))
      {
        current++;
      }

      try
      {
        std::size_t count = std::stoull(line.substr(start, current - start));
        return {true, count, current};
      }
      catch (...)
      {
        return {false, 0, current};
      }
    }
  };

  class LineProcessor
  {
  public:
    bool operator()(const std::string& line, Polygon& poly) const
    {
      CountParser count_parser;
      auto count_result = count_parser(line);
      if (!count_result.success || count_result.count < 3)
      {
        return false;
      }

      PointsParser points_parser;
      auto points_result = points_parser(line, count_result.new_pos, count_result.count);
      if (!points_result.success || points_result.points.size() != count_result.count)
      {
        return false;
      }

      WhitespaceSkipper skipper;
      std::size_t final_pos = skipper(line, points_result.new_pos);
      if (final_pos != line.size())
      {
        return false;
      }

      poly.points = std::move(points_result.points);
      return true;
    }
  };

  class StreamReader
  {
  public:
    StreamReader(std::istream& in, Polygon& poly):
      in_(in),
      poly_(poly)
    {}

    bool readNextLine()
    {
      std::string line;
      if (!std::getline(in_, line))
      {
        return false;
      }

      LineProcessor processor;
      return processor(line, poly_);
    }

  private:
    std::istream& in_;
    Polygon& poly_;
  };

  PolygonIterator& PolygonIterator::operator++()
  {
    if (!in_)
    {
      end_ = true;
      return *this;
    }

    StreamReader reader(*in_, poly_);
    if (reader.readNextLine())
    {
      return *this;
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
