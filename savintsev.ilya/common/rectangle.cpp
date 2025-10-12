#include "rectangle.hpp"
#include <cmath>
#include <stdexcept>

savintsev::Rectangle::Rectangle(point_t lhs, point_t rhs, std::string name):
  Shape(std::move(name)),
  l_(lhs),
  r_(rhs)
{
  if (l_.x >= r_.x || l_.y >= r_.y)
  {
    throw std::runtime_error("Invalid argumets for Rectangle");
  }
}

savintsev::rectangle_t savintsev::Rectangle::get_frame_rect() const
{
  double centerByX = l_.x + ((r_.x - l_.x) / 2.);
  double centerByY = l_.y + ((r_.y - l_.y) / 2.);
  return {r_.x - l_.x, r_.y - l_.y, {centerByX, centerByY}};
}

size_t savintsev::Rectangle::get_all_points(point_t * ps) const
{
  if (ps)
  {
    ps[0] = l_;
    ps[1] = r_;
  }
  return 2;
}

void savintsev::Rectangle::move(point_t p)
{
  point_t center = get_frame_rect().pos;
  double moveByX = p.x - center.x;
  double moveByY = p.y - center.y;
  l_ = {l_.x + moveByX, l_.y + moveByY};
  r_ = {r_.x + moveByX, r_.y + moveByY};
}

void savintsev::Rectangle::move(double x, double y)
{
  l_ = {l_.x + x, l_.y + y};
  r_ = {r_.x + x, r_.y + y};
}

savintsev::Shape * savintsev::Rectangle::clone() const
{
  return new Rectangle(*this);
}

void savintsev::Rectangle::unsafe_scale(double k) noexcept
{
  point_t center = get_frame_rect().pos;
  l_ = {center.x - (center.x - l_.x) * k, center.y - (center.y - l_.y) * k};
  r_ = {center.x + (r_.x - center.x) * k, center.y + (r_.y - center.y) * k};
}
