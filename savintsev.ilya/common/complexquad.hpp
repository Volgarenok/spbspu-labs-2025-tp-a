#ifndef COMPLEXQUAD_HPP
#define COMPLEXQUAD_HPP
#include <string>
#include "shape.hpp"

namespace savintsev
{
  class Complexquad final: public Shape
  {
  public:
    Complexquad(point_t p1, point_t p2, point_t p3, point_t p4, std::string name = "");

    rectangle_t get_frame_rect() const override;
    size_t get_all_points(point_t * ps) const override;

    void unsafe_scale(double k) noexcept override;
    void move(point_t p) override;
    void move(double x, double y) override;

    Shape * clone() const override;

  private:
    point_t p1_;
    point_t p2_;
    point_t p3_;
    point_t p4_;
  };
}
#endif
