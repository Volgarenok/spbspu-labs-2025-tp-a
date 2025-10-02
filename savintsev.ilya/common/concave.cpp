#include "concave.hpp"
#include <cmath>
#include <stdexcept>
#include "geometry-utils.hpp"

savintsev::Concave::Concave(point_t p1, point_t p2, point_t p3, point_t p4, std::string name):
  Shape(std::move(name)),
  p1_(p1),
  p2_(p2),
  p3_(p3),
  p4_(p4)
{
  if (!isTriangle(p1_, p2_, p3_) || !isPointInTriangle(p1_, p2_, p3_, p4_))
  {
    throw std::runtime_error("Invalid argumets for Concave");
  }
}

savintsev::rectangle_t savintsev::Concave::get_frame_rect() const
{
  double mostLeft = std::fmin(p1_.x, std::fmin(p2_.x, p3_.x));
  double mostRight = std::fmax(p1_.x, std::fmax(p2_.x, p3_.x));
  double mostLow = std::fmin(p1_.y, std::fmin(p2_.y, p3_.y));
  double mostHigh = std::fmax(p1_.y, std::fmax(p2_.y, p3_.y));
  point_t center = {mostLeft + (mostRight - mostLeft) / 2, mostLow + (mostHigh - mostLow) / 2};
  return {mostRight - mostLeft, mostHigh - mostLow, center};
}

size_t savintsev::Concave::get_all_points(point_t * ps) const
{
  if (ps)
  {
    ps[0] = p1_;
    ps[1] = p2_;
    ps[2] = p3_;
    ps[3] = p4_;
  }
  return 4;
}

void savintsev::Concave::move(point_t p)
{
  double moveByX = p.x - p4_.x;
  double moveByY = p.y - p4_.y;
  p1_ = {p1_.x + moveByX, p1_.y + moveByY};
  p2_ = {p2_.x + moveByX, p2_.y + moveByY};
  p3_ = {p3_.x + moveByX, p3_.y + moveByY};
  p4_ = {p4_.x + moveByX, p4_.y + moveByY};
}

void savintsev::Concave::move(double x, double y)
{
  p1_ = {p1_.x + x, p1_.y + y};
  p2_ = {p2_.x + x, p2_.y + y};
  p3_ = {p3_.x + x, p3_.y + y};
  p4_ = {p4_.x + x, p4_.y + y};
}

savintsev::Shape * savintsev::Concave::clone() const
{
  return new Concave(*this);
}

void savintsev::Concave::unsafe_scale(double k) noexcept
{
  p1_ = {p4_.x - (p4_.x - p1_.x) * k, p4_.y - (p4_.y - p1_.y) * k};
  p2_ = {p4_.x - (p4_.x - p2_.x) * k, p4_.y - (p4_.y - p2_.y) * k};
  p3_ = {p4_.x - (p4_.x - p3_.x) * k, p4_.y - (p4_.y - p3_.y) * k};
}
