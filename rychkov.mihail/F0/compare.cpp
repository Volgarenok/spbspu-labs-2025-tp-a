#include "compare.hpp"

#include "content.hpp"

bool rychkov::NameCompare::operator()(const std::string& lhs, const std::string& rhs) const
{
  return lhs < rhs;
}

bool rychkov::NameCompare::operator()(const operator_value& lhs, const operator_value& rhs) const
{
  return operator()(lhs[0], rhs[0]);
}
bool rychkov::NameCompare::operator()(const operator_value& lhs, const std::string& rhs) const
{
  return operator()(lhs[0], rhs);
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const operator_value& rhs) const
{
  return operator()(lhs, rhs[0]);
}

bool rychkov::NameCompare::operator()(const Operator& lhs, const Operator& rhs) const
{
  return lhs.token < rhs.token;
}
bool rychkov::NameCompare::operator()(const Operator& lhs, const std::string& rhs) const
{
  return lhs.token < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const Operator& rhs) const
{
  return lhs < rhs.token;
}
