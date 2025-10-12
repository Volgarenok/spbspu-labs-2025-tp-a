#include "shape.hpp"
#include <stdexcept>

savintsev::Shape::Shape(std::string name):
  name_(name)
{}

void savintsev::Shape::scale(double k)
{
  if (k <= 0)
  {
    throw std::runtime_error("Invalid scale ratio");
  }
  unsafe_scale(k);
}

std::string savintsev::Shape::get_name() const noexcept
{
  return name_;
}

void savintsev::Shape::set_name(std::string name)
{
  name_ = name;
}
