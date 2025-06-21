#include "compare.hpp"

bool rychkov::NameCompare::operator()(const struct_value& lhs, const struct_value& rhs) const
{
  return (lhs.first.name < rhs.first.name) || ((lhs.first.name == rhs.first.name) && (lhs.second > rhs.second));
}

bool rychkov::NameCompare::operator()(const base_type_value& lhs, const base_type_value& rhs) const
{
  return (lhs.first.name < rhs.first.name) || ((lhs.first.name == rhs.first.name) && (lhs.second > rhs.second));
}
bool rychkov::NameCompare::operator()(const base_type_value& lhs, const std::string& rhs) const
{
  return lhs.first.name < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const base_type_value& rhs) const
{
  return lhs < rhs.first.name;
}

bool rychkov::NameCompare::operator()(const variable_value& lhs, const variable_value& rhs) const
{
  return (lhs.first.name < rhs.first.name) || ((lhs.first.name == rhs.first.name) && (lhs.second > rhs.second));
}
bool rychkov::NameCompare::operator()(const variable_value& lhs, const std::string& rhs) const
{
  return lhs.first.name < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const variable_value& rhs) const
{
  return lhs < rhs.first.name;
}

bool rychkov::NameCompare::operator()(const entities::Variable& lhs, const entities::Variable& rhs) const
{
  return lhs.name < rhs.name;
}
bool rychkov::NameCompare::operator()(const entities::Variable& lhs, const std::string& rhs) const
{
  return lhs.name < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const entities::Variable& rhs) const
{
  return lhs < rhs.name;
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

bool rychkov::NameCompare::operator()(const Macro& lhs, const Macro& rhs) const
{
  return lhs.name < rhs.name;
}
bool rychkov::NameCompare::operator()(const Macro& lhs, const std::string& rhs) const
{
  return lhs.name < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const Macro& rhs) const
{
  return lhs < rhs.name;
}
