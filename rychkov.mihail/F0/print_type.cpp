#include "print_content.hpp"

#include <iostream>

std::ostream& rychkov::details::print_left_parenthesis(std::ostream& out, const typing::Type& parent)
{
  if (parent.base == nullptr)
  {
    return out;
  }
  if ((parent.category == typing::Type::Array) && (parent.base->category == typing::Type::Array))
  {
    return out;
  }
  if ((parent.base->category == typing::Type::Function) || (parent.base->category == typing::Type::Array))
  {
    out << '(';
  }
  return out;
}
std::ostream& rychkov::details::print_right_parenthesis(std::ostream& out, const typing::Type& parent)
{
  if (parent.base == nullptr)
  {
    return out;
  }
  if ((parent.category == typing::Type::Array) && (parent.base->category == typing::Type::Array))
  {
    return out;
  }
  if ((parent.base->category == typing::Type::Function) || (parent.base->category == typing::Type::Array))
  {
    out << ')';
  }
  return out;
}
std::ostream& rychkov::print_left(std::ostream& out, const typing::Type& type)
{
  if (type.base != nullptr)
  {
    print_left(out, *type.base);
  }
  switch (type.category)
  {
  case typing::Type::Struct:
    out << type.name;
    break;
  case typing::Type::Int:
    if (type.is_signed)
    {
      out << "signed ";
    }
    if (type.is_unsigned)
    {
      out << "unsigned ";
    }
    switch (type.length_category)
    {
    case typing::Type::SHORT:
      out << "short ";
      break;
    case typing::Type::LONG:
      out << "long ";
      break;
    case typing::Type::LONG_LONG:
      out << "long long ";
      break;
    }
    out << type.name;
    break;
  case typing::Type::Pointer:
    details::print_left_parenthesis(out, type);
    out << '*';
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  case typing::Type::Array:
    details::print_left_parenthesis(out, type);
    break;
  case typing::Type::Combination:
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  }
  return out;
}
std::ostream& rychkov::print_right(std::ostream& out, const typing::Type& type)
{
  if (type.category == typing::Type::Function)
  {
    out << '(';
    char comma[3] = "\0\0";
    for (const typing::Type& i: type.function_parameters)
    {
      out << comma << i;
      comma[0] = ',';
      comma[1] = ' ';
    }
    out << ')';
    details::print_right_parenthesis(out, type);
  }
  else if (type.category == typing::Type::Pointer)
  {
    details::print_right_parenthesis(out, type);
  }
  else if (type.category == typing::Type::Array)
  {
    out << '[';
    if (type.array_has_length)
    {
      out << type.array_length;
    }
    out << ']';
    details::print_right_parenthesis(out, type);
  }
  if (type.base != nullptr)
  {
    print_right(out, *type.base);
  }
  return out;
}
std::ostream& rychkov::operator<<(std::ostream& out, const typing::Type& type)
{
  print_left(out, type);
  print_right(out, type);
  return out;
}
