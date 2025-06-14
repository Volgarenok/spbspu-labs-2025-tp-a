#include "code_parsers.hpp"

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
  return lhs[0].token < rhs[0].token;
}
bool rychkov::NameCompare::operator()(const operator_value& lhs, const std::string& rhs) const
{
  return lhs[0].token < rhs;
}
bool rychkov::NameCompare::operator()(const std::string& lhs, const operator_value& rhs) const
{
  return lhs < rhs[0].token;
}
