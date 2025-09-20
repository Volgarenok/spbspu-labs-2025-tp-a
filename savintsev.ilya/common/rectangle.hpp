#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"
#include "complexquad.hpp"

namespace savintsev
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(point_t lhs, point_t rhs, std::string name = "");
    Shape * clone() const override;

    rectangle_t get_frame_rect() const override;
    size_t get_all_points(point_t * ps) const override;

    void unsafe_scale(double k) noexcept override;
    void move(point_t p) override;
    void move(double x, double y) override;
  private:
    point_t l_;
    point_t r_;
  };
}
#endif
