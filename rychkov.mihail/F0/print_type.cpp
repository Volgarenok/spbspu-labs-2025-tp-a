#include "print_content.hpp"

#include <iostream>

std::ostream& rychkov::details::print_left_parenthesis(std::ostream& out, const typing::Type& parent)
{
  if (parent.base == nullptr)
  {
    return out;
  }
  if ((parent.category == typing::ARRAY) && (parent.base->category == typing::ARRAY))
  {
    return out;
  }
  if ((parent.base->category == typing::FUNCTION) || (parent.base->category == typing::ARRAY))
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
  if ((parent.category == typing::ARRAY) && (parent.base->category == typing::ARRAY))
  {
    return out;
  }
  if ((parent.base->category == typing::FUNCTION) || (parent.base->category == typing::ARRAY))
  {
    out << ')';
  }
  return out;
}
std::ostream& rychkov::typing::print_left(std::ostream& out, const typing::Type& type)
{
  if (type.base != nullptr)
  {
    print_left(out, *type.base);
  }
  switch (type.category)
  {
  case typing::ENUM:
  case typing::STRUCT:
    out << type.name;
    break;
  case typing::BASIC:
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
    case typing::SHORT:
      out << "short ";
      break;
    case typing::LONG:
      out << "long ";
      break;
    case typing::LONG_LONG:
      out << "long long ";
      break;
    default:
      break;
    }
    out << type.name;
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  case typing::POINTER:
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
  case typing::ARRAY:
    details::print_left_parenthesis(out, type);
    break;
  case typing::COMBINATION:
    if (type.is_const)
    {
      out << " const";
    }
    if (type.is_volatile)
    {
      out << " volatile";
    }
    break;
  case typing::FUNCTION:
    break;
  }
  return out;
}
std::ostream& rychkov::typing::print_right(std::ostream& out, const typing::Type& type)
{
  if (type.category == typing::FUNCTION)
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
  else if (type.category == typing::POINTER)
  {
    details::print_right_parenthesis(out, type);
  }
  else if (type.category == typing::ARRAY)
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
std::ostream& rychkov::typing::operator<<(std::ostream& out, const typing::Type& type)
{
  print_left(out, type);
  print_right(out, type);
  return out;
}
