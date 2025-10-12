#include "complexquad.hpp"
#include <algorithm>
#include <stdexcept>
#include "geometry-utils.hpp"

savintsev::Complexquad::Complexquad(point_t p1, point_t p2, point_t p3, point_t p4, std::string name):
  Shape(std::move(name)),
  p1_(p1),
  p2_(p2),
  p3_(p3),
  p4_(p4)
{
  point_t center = {0.0, 0.0};
  if (!findLinesIntersect(p1_, p2_, p3_, p4_, center) || !isTriangle(p1_, p4_, center) || !isTriangle(p2_, p3_, center))
  {
    throw std::runtime_error("Invalid argumets for Complexquad");
  }
}

savintsev::rectangle_t savintsev::Complexquad::get_frame_rect() const
{
  double mostLeft = std::min({p1_.x, p2_.x, p3_.x, p4_.x});
  double mostRight = std::max({p1_.x, p2_.x, p3_.x, p4_.x});
  double mostLow = std::min({p1_.y, p2_.y, p3_.y, p4_.y});
  double mostHigh = std::max({p1_.y, p2_.y, p3_.y, p4_.y});
  point_t center = {mostLeft + (mostRight - mostLeft) / 2, mostLow + (mostHigh - mostLow) / 2};
  return {mostRight - mostLeft, mostHigh - mostLow, center};
}

size_t savintsev::Complexquad::get_all_points(point_t * ps) const
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

void savintsev::Complexquad::move(point_t p)
{
  point_t center = {0.0, 0.0};
  findLinesIntersect(p1_, p2_, p3_, p4_, center);
  double moveByX = p.x - center.x;
  double moveByY = p.y - center.y;
  p1_ = {p1_.x + moveByX, p1_.y + moveByY};
  p2_ = {p2_.x + moveByX, p2_.y + moveByY};
  p3_ = {p3_.x + moveByX, p3_.y + moveByY};
  p4_ = {p4_.x + moveByX, p4_.y + moveByY};
}

void savintsev::Complexquad::move(double x, double y)
{
  p1_ = {p1_.x + x, p1_.y + y};
  p2_ = {p2_.x + x, p2_.y + y};
  p3_ = {p3_.x + x, p3_.y + y};
  p4_ = {p4_.x + x, p4_.y + y};
}

savintsev::Shape * savintsev::Complexquad::clone() const
{
  return new Complexquad(*this);
}

void savintsev::Complexquad::unsafe_scale(double k) noexcept
{
  point_t center = {0.0, 0.0};
  findLinesIntersect(p1_, p2_, p3_, p4_, center);
  p1_ = {center.x - (center.x - p1_.x) * k, center.y - (center.y - p1_.y) * k};
  p2_ = {center.x - (center.x - p2_.x) * k, center.y - (center.y - p2_.y) * k};
  p3_ = {center.x - (center.x - p3_.x) * k, center.y - (center.y - p3_.y) * k};
  p4_ = {center.x - (center.x - p4_.x) * k, center.y - (center.y - p4_.y) * k};
}
