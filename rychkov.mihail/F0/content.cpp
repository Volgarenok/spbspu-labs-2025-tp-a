#include "content.hpp"

bool rychkov::typing::Type::empty() const noexcept
{
  return (category == Combination) && !is_const && !is_volatile && !is_signed && !is_unsigned;
}
rychkov::entities::Expression::Expression():
  operation{nullptr}
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
  result_type{},
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
rychkov::entities::Expression::Expression(const Operator* op, typing::Type result, std::vector< operand > opers):
  operation{op},
  result_type{result},
  operands(std::move(opers))
{}
rychkov::entities::Body::Body():
  data{{}}
{}

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
bool rychkov::entities::is_body(const entities::Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && std::holds_alternative< entities::Body >(expr.operands[0]);
}
bool rychkov::entities::is_decl(const entities::Expression& expr)
{
  return (expr.operation == nullptr) && !expr.operands.empty()
      && std::holds_alternative< entities::Declaration >(expr.operands[0]);
}
