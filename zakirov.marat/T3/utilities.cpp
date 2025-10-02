#include "utilities.hpp"
#include <string>

  bool zakirov::odd_polygon_pred(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size() % 2 == 1;
  }

  bool zakirov::even_polygon_pred(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size() % 2 == 0;
  }

  bool zakirov::less_area_pred(double area_base, double area_other)
  {
    return area_other < area_base;
  }

  bool zakirov::equal_vertexes_pred(const zakirov::Polygon & polygon, size_t vertex_base)
  {
    return polygon.points_.size() == vertex_base;
  }

  size_t zakirov::get_vertex(const zakirov::Polygon & polygon)
  {
    return polygon.points_.size();
  }

  double zakirov::count_area(const zakirov::Polygon & plgn)
  {
    double result = 0;
    for (size_t i = 1; i < plgn.points_.size(); ++i)
    {
      result += plgn.points_[i-1].x_ * plgn.points_[i].y_ - plgn.points_[i - 1].y_ * plgn.points_[i].x_;
    }

    result += plgn.points_[plgn.points_.size() - 1].x_ * plgn.points_[0].y_ - plgn.points_[plgn.points_.size() - 1].y_ * plgn.points_[0].x_;
    result *= 0.5;
    return std::abs(result);
  }

  int zakirov::extract_num_until(const std::string & str, size_t start_pos, char el)
  {
    auto it = std::find(str.begin() + start_pos, str.end(), el);
    std::string substr(str.begin() + start_pos, it);
    if (!std::all_of(substr.begin(), substr.end(), ::isdigit))
    {
      throw std::logic_error("<NOT A COORDINATE>");
    }

    return std::stoi(substr);
  }

  zakirov::Point zakirov::parce_point (const std::string & str)
  {
    zakirov::Point result{0, 0};
    size_t pos = 0;
    if (str[pos] != '(')
    {
      throw std::logic_error("<NOT A POINT>");
    }

    ++pos;
    result.x_ = extract_num_until(str, pos, ';');
    pos = str.find(";", pos) + 1;
    result.y_ = extract_num_until(str, pos, ')');
    pos = str.find(")", pos) + 1;

    if (pos != str.size())
    {
      throw std::logic_error("<NOT A POINT>");
    }

    return result;
  }
