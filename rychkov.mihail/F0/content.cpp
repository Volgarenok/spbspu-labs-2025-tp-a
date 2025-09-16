#include "content.hpp"

#include <utility>

bool rychkov::typing::Type::empty() const noexcept
{
  return (category == COMBINATION) && !is_const && !is_volatile && !is_signed && !is_unsigned;
}
bool rychkov::typing::Type::ready() const noexcept
{
  return (category != COMBINATION) || (base != nullptr);
}
rychkov::entities::Expression::Expression():
  operation{nullptr},
  result_type{}
{}
rychkov::entities::Expression::Expression(Variable var):
  operation{nullptr},
  result_type{var.type},
  operands{std::move(var)}
{}
rychkov::entities::Expression::Expression(Declaration decl):
  operation{nullptr},
  result_type{},
  operands{std::move(decl)}
{}
rychkov::entities::Expression::Expression(Literal lit):
  operation{nullptr},
  result_type{lit.result_type},
  operands{std::move(lit)}
{}
rychkov::entities::Expression::Expression(CastOperation cast):
  operation{nullptr},
  result_type{cast.to},
  operands{std::move(cast)}
{}
rychkov::entities::Expression::Expression(Body body):
  operation{nullptr},
  result_type{},
  operands{std::move(body)}
{}
rychkov::entities::Expression::Expression(const Operator* op, std::vector< operand > opers):
  operation{op},
  result_type{},
  operands(std::move(opers))
{}
rychkov::entities::Body::Body():
  data{{}}
{}

const std::string& rychkov::entities::get_name(const std::string& identity)
{
  return identity;
}

bool rychkov::entities::Expression::empty() const noexcept
{
  return (operation == nullptr) && operands.empty();
}
bool rychkov::entities::Expression::full() const noexcept
{
  if (operation == nullptr)
  {
    return !operands.empty();
  }
  if (operation->type == -1)
  {
    return false;
  }
  return static_cast< std::ptrdiff_t >(operands.size()) >= operation->type;
}
bool rychkov::entities::is_body(const Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && boost::variant2::holds_alternative< Body >(expr.operands[0]);
}
bool rychkov::entities::is_decl(const Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && boost::variant2::holds_alternative< Declaration >(expr.operands[0]);
}
bool rychkov::entities::is_operator(const Expression& expr)
{
  return expr.operation != nullptr;
}
bool rychkov::entities::is_bridge(const Expression& expr)
{
  return expr.operation == nullptr;
}

void rychkov::entities::remove_bridge(Expression& expr)
{
  if (!is_bridge(expr) || (expr.operands.size() != 1))
  {
    return;
  }
  if (!boost::variant2::holds_alternative< DynMemWrapper< Expression > >(expr.operands[0]))
  {
    return;
  }
  Expression temp = std::move(*boost::variant2::get< DynMemWrapper< Expression > >(expr.operands[0]));
  expr = std::move(temp);
}

const rychkov::typing::Type* rychkov::entities::get_type(const Expression::operand& operand)
{
  if (boost::variant2::holds_alternative< DynMemWrapper< Expression > >(operand))
  {
    return &boost::variant2::get< DynMemWrapper< Expression > >(operand)->result_type;
  }
  else if (boost::variant2::holds_alternative< Variable >(operand))
  {
    return &boost::variant2::get< Variable >(operand).type;
  }
  else if (boost::variant2::holds_alternative< Literal >(operand))
  {
    return &boost::variant2::get< Literal >(operand).result_type;
  }
  else if (boost::variant2::holds_alternative< CastOperation >(operand))
  {
    return &boost::variant2::get< Variable >(operand).type;
  }
  return nullptr;
}
bool rychkov::entities::is_lvalue(const Expression::operand& operand)
{
  if (boost::variant2::holds_alternative< DynMemWrapper< Expression > >(operand))
  {
    return is_lvalue(&*boost::variant2::get< DynMemWrapper< Expression > >(operand));
  }
  return boost::variant2::holds_alternative< entities::Variable >(operand);
}
bool rychkov::entities::is_lvalue(const Expression* expr)
{
  if ((expr->operation == nullptr) || (expr->operation->token == ","))
  {
    return !expr->operands.empty() && is_lvalue(expr->operands.back());
  }
  return expr->operation->produce_lvalue;
}

bool rychkov::typing::exact_cv(const Type& dest, const Type& src)
{
  return (dest.is_const == src.is_const) && (dest.is_volatile == src.is_volatile) && (check_cast(dest, src) == EXACT);
}
rychkov::typing::MatchType rychkov::typing::check_cast(const Type& dest, const Type& src)
{
  if (dest.category != src.category)
  {
    if (is_integer(&dest) && (is_iterable(&src)))
    {
      return IMPLICIT;
    }
    if (is_pointer(&dest) && is_array(&src))
    {
      return check_cast(*dest.base, *src.base) == EXACT ? IMPLICIT : NO_CAST;
    }
    if (is_pointer(&dest) && is_function(&src))
    {
      return exact_cv(*dest.base, src) ? IMPLICIT : NO_CAST;
    }
    if (is_integer(&dest) && is_enum(&src))
    {
      return IMPLICIT;
    }
    return NO_CAST;
  }

  switch (dest.category)
  {
  case STRUCT:
  case ENUM:
    return dest.name == src.name ? EXACT : NO_CAST;
  case BASIC:
    return EXACT;
  case FUNCTION:
    return exact_cv(*dest.base, *src.base) && std::equal(dest.function_parameters.begin(),
          dest.function_parameters.end(), src.function_parameters.begin(), src.function_parameters.end(),
          exact_cv) ? EXACT : NO_CAST;
  case POINTER:
    if ((!dest.base->is_const && src.base->is_const) || (!dest.base->is_volatile && src.base->is_volatile))
    {
      return NO_CAST;
    }
    return check_cast(*dest.base, *src.base) == EXACT ? EXACT : NO_CAST;
  case ARRAY:
    if (dest.array_has_length && (dest.array_length != src.array_length))
    {
      return NO_CAST;
    }
    return check_cast(*dest.base, *src.base) == EXACT ? EXACT : NO_CAST;
  default:
    return NO_CAST;
  }
}
rychkov::typing::MatchType rychkov::typing::check_overload(const Type& function, const std::vector< Type >& args)
{
  if (function.function_parameters.size() != args.size())
  {
    return NO_CAST;
  }
  MatchType result = EXACT;
  for (size_t i = 0; i < args.size(); i++)
  {
    switch (check_cast(function.function_parameters[i], args[i]))
    {
    case EXACT:
      break;
    case IMPLICIT:
      result = IMPLICIT;
      break;
    case NO_CAST:
      return NO_CAST;
    }
  }
  return result;
}

const rychkov::typing::Type* rychkov::typing::largest_integer(const Type& lhs, const Type& rhs)
{
  if (!is_integer(&lhs) || !is_integer(&rhs))
  {
    return nullptr;
  }
  return &lhs;
}
const rychkov::typing::Type* rychkov::typing::common_type(const Type& lhs, const Type& rhs)
{
  if ((!is_basic(&lhs) && !is_pointer(&lhs) && !is_array(&lhs)) || !is_basic(&rhs))
  {
    return nullptr;
  }
  return &lhs;
}
bool rychkov::typing::pointer_comparable(const Type& lhs, const Type& rhs)
{
  return (is_pointer(&lhs) || is_array(&lhs)) && (is_pointer(&rhs) || is_array(&rhs))
      && (check_cast(lhs, rhs) == EXACT);
}

bool rychkov::typing::is_basic(const Type* type)
{
  return (type != nullptr) && (type->category == BASIC);
}
bool rychkov::typing::is_enum(const Type* type)
{
  return (type != nullptr) && (type->category == ENUM);
}
bool rychkov::typing::is_function(const Type* type)
{
  return (type != nullptr) && (type->category == FUNCTION);
}
bool rychkov::typing::is_pointer(const Type* type)
{
  return (type != nullptr) && (type->category == POINTER);
}
bool rychkov::typing::is_array(const Type* type)
{
  return (type != nullptr) && (type->category == ARRAY);
}
bool rychkov::typing::is_callable(const Type* type)
{
  return is_function(type) || (is_pointer(type) && is_function(&*type->base));
}
bool rychkov::typing::is_iterable(const Type* type)
{
  return is_pointer(type) || is_array(type);
}
bool rychkov::typing::is_integer(const Type* type)
{
  return is_basic(type) && ((type->name == "int") || (type->name == "char"));
}
