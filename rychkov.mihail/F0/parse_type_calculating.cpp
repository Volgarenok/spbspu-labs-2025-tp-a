#include "cparser.hpp"

#include <iostream>
#include "print_content.hpp"

void rychkov::CParser::calculate_type(CParseContext& context, entities::Expression& expr)
{
  expr.result_type = {};
  if (entities::is_operator(expr) && (expr.operation != &comma))
  {
    if (!expr.full())
    {
      return;
    }
    if (expr.operation->require_lvalue && !entities::is_lvalue(expr.operands[0]))
    {
      log(context, "operator" + expr.operation->token + " requires lvalue");
      return;
    }
    for (const entities::Expression::operand& arg: expr.operands)
    {
      const typing::Type* type_p = entities::get_type(arg);
      if (type_p == nullptr)
      {
        return;
      }
      if (type_p->name == "void")
      {
        log(context, "operator cannot be applied to void");
        return;
      }
    }
    switch (expr.operation->category)
    {
    case Operator::FIELD_ACCESS:
      expr.result_type = *entities::get_type(expr.operands[1]);
      return;
    case Operator::SPECIAL:
      if (expr.operation == &brackets)
      {
        const typing::Type* iterable = entities::get_type(expr.operands[0]);
        if (!typing::is_iterable(iterable))
        {
          log(context, "operator[] must be applied to iterable type");
        }
        if (!typing::is_integer(entities::get_type(expr.operands[1])))
        {
          log(context, "operator[] argument must be integer");
        }
        else
        {
          expr.result_type = *iterable->base;
        }
      }
      if (expr.operation == &inline_if)
      {
        const typing::Type* main_body = entities::get_type(expr.operands[1]);
        const typing::Type* else_body = entities::get_type(expr.operands[2]);
        const typing::Type* common = typing::common_type(*main_body, *else_body);
        if (common == nullptr)
        {
          log(context, "operator?: must be applied to basic types");
          return;
        }
        require_type(context, expr.operands[0], {"int", typing::BASIC});
        require_type(context, expr.operands[1], *common);
        require_type(context, expr.operands[2], *common);
        expr.result_type = *common;
      }
      return;
    case Operator::DEREFERENCE:
    {
      const typing::Type* iterable = entities::get_type(expr.operands[0]);
      if (!typing::is_iterable(iterable))
      {
        start_log(context) << "cannot dereference " << *iterable;
        finish_log(context);
        return;
      }
      expr.result_type = *iterable->base;
      return;
    }
    case Operator::ADDRESSOF:
      expr.result_type = {{}, typing::POINTER, *entities::get_type(expr.operands[0])};
      return;
    case Operator::INCREMENT:
    {
      const typing::Type* child = entities::get_type(expr.operands[0]);
      if (!typing::is_integer(child) && !(is_pointer(child)))
      {
        start_log(context) << "operator" << expr.operation->token << " cannot be applied to " << *child;
        finish_log(context);
        return;
      }
      expr.result_type = *child;
      return;
    }
    case Operator::ARITHMETIC:
    {
      if (expr.operation->type == Operator::BINARY)
      {
        break;
      }
      const typing::Type* basic = entities::get_type(expr.operands[0]);
      if (!typing::is_basic(basic))
      {
        start_log(context) << "operator" << expr.operation->token << " cannot be applied to " << *basic;
        finish_log(context);
        return;
      }
      expr.result_type = *basic;
      return;
    }
    case Operator::BIT:
    {
      if (expr.operation->type == Operator::BINARY)
      {
        break;
      }
      const typing::Type* integer = entities::get_type(expr.operands[0]);
      if (!typing::is_integer(integer))
      {
        log(context, "operator~ requires an integer");
        return;
      }
      expr.result_type = *integer;
      return;
    }
    case Operator::LOGIC:
      if (expr.operation->type == Operator::BINARY)
      {
        break;
      }
      expr.result_type = {"int", typing::BASIC};
      require_type(context, expr.operands[0], expr.result_type);
      return;
    default:
      break;
    }

    const typing::Type* lhs = entities::get_type(expr.operands[0]);
    const typing::Type* rhs = entities::get_type(expr.operands[1]);
    switch (expr.operation->category)
    {
    case Operator::ARITHMETIC:
    {
      const typing::Type* common = typing::common_type(*lhs, *rhs);
      if (common == nullptr)
      {
        start_log(context) << "operator" << expr.operation->token << " must be applied to basic types";
        finish_log(context);
        return;
      }
      if (expr.operation->require_lvalue)
      {
        require_type(context, expr.operands[1], *lhs);
        expr.result_type = *lhs;
        return;
      }
      require_type(context, expr.operands[0], *common);
      require_type(context, expr.operands[1], *common);
      expr.result_type = *common;
      return;
    }
    case Operator::LOGIC:
      expr.result_type = {"int", typing::BASIC};
      require_type(context, expr.operands[0], expr.result_type);
      require_type(context, expr.operands[1], expr.result_type);
      return;
    case Operator::COMPARE:
    {
      expr.result_type = {"int", typing::BASIC};
      const typing::Type* common = typing::common_type(*lhs, *rhs);
      if (common == nullptr)
      {
        if (pointer_comparable(*lhs, *rhs))
        {
          return;
        }
        start_log(context) << "operator" << expr.operation->token << " must be applied to basic types";
        finish_log(context);
        return;
      }
      require_type(context, expr.operands[0], *common);
      require_type(context, expr.operands[1], *common);
      return;
    }
    case Operator::MODULUS:
    case Operator::BIT:
    {
      const typing::Type* common = typing::largest_integer(*lhs, *rhs);
      if (common == nullptr)
      {
        start_log(context) << "operator" << expr.operation->token << " must be applied to integers";
        finish_log(context);
        return;
      }
      if (expr.operation->require_lvalue)
      {
        require_type(context, expr.operands[1], *lhs);
        expr.result_type = *lhs;
        return;
      }
      require_type(context, expr.operands[0], *common);
      require_type(context, expr.operands[1], *common);
      expr.result_type = *common;
      return;
    }
    case Operator::ASSIGN:
      expr.result_type = *lhs;
      require_type(context, expr.operands[1], *lhs);
      return;
    default:
      break;
    }
    return;
  }
  if (!expr.operands.empty())
  {
    const typing::Type* temp = entities::get_type(expr.operands.back());
    expr.result_type = (temp == nullptr ? typing::Type{} : *temp);
  }
}
void rychkov::CParser::require_type(CParseContext& context, entities::Expression::operand& expr,
    const typing::Type& type)
{
  const typing::Type* from = entities::get_type(expr);
  if (from == nullptr)
  {
    log(context, "cannot use this expression as type");
    return;
  }
  switch (typing::check_cast(type, *from))
  {
  case typing::EXACT:
    break;
  case typing::IMPLICIT:
  {
    entities::CastOperation temp = {type, false, entities::Expression{nullptr, {std::move(expr)}}};
    expr = std::move(temp);
    break;
  }
  case typing::NO_CAST:
    start_log(context) << "cannot cast " << *from << " to " << type;
    finish_log(context);
    break;
  }
}
void rychkov::CParser::require_type(CParseContext& context, entities::Expression& expr,
    const typing::Type& type)
{
  if (!expr.result_type.ready())
  {
    log(context, "cannot use this expression as type");
    return;
  }
  switch (typing::check_cast(type, expr.result_type))
  {
  case typing::EXACT:
    break;
  case typing::IMPLICIT:
  {
    entities::CastOperation temp = {type, false, std::move(expr)};
    expr = std::move(temp);
    calculate_type(context, *boost::variant2::get< entities::CastOperation >(expr.operands[0]).expr);
    break;
  }
  case typing::NO_CAST:
    start_log(context) << "cannot cast " << expr.result_type << " to " << type;
    finish_log(context);
    break;
  }
}
std::pair< const rychkov::entities::Variable*, rychkov::typing::MatchType > rychkov::CParser::find_overload
    (const std::string& name, const std::vector< typing::Type >& args)
{
  using Iter = decltype(defined_functions)::iterator;
  std::pair< Iter, Iter > range = defined_functions.equal_range(name);
  for (; range.first != range.second; ++range.first)
  {
    if (typing::check_overload(range.first->type, args) == typing::EXACT)
    {
      return {&*range.first, typing::EXACT};
    }
  }
  return {nullptr, typing::NO_CAST};
}
