#include "cparser.hpp"

#include <iostream>
#include "print_content.hpp"

void rychkov::CParser::calculate_type(CParseContext& context, entities::Expression& expr)
{
  expr.result_type = nullptr;
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
      expr.result_type = entities::get_type(expr.operands[1]);
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
          expr.result_type = &*iterable->base;
        }
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
      expr.result_type = &*iterable->base;
      return;
    }
    case Operator::ADDRESSOF:
      expr.buffered_ = {{}, typing::POINTER, *entities::get_type(expr.operands[0])};
      expr.result_type = &expr.buffered_;
      return;
    case Operator::INCREMENT:
      expr.result_type = entities::get_type(expr.operands[0]);
      return;
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
      expr.result_type = basic;
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
      expr.result_type = integer;
      return;
    }
    case Operator::LOGIC:
      if (expr.operation->type == Operator::BINARY)
      {
        break;
      }
      expr.buffered_ = {"int", typing::BASIC};
      expr.result_type = &expr.buffered_;
      require_type(context, expr.operands[0], expr.buffered_);
      return;
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
        expr.result_type = lhs;
        return;
      }
      require_type(context, expr.operands[0], *common);
      require_type(context, expr.operands[1], *common);
      expr.result_type = common;
      return;
    }
    case Operator::LOGIC:
      expr.buffered_ = {"int", typing::BASIC};
      expr.result_type = &expr.buffered_;
      require_type(context, expr.operands[0], expr.buffered_);
      require_type(context, expr.operands[1], expr.buffered_);
      return;
    case Operator::COMPARE:
    {
      expr.buffered_ = {"int", typing::BASIC};
      expr.result_type = &expr.buffered_;
      const typing::Type* common = typing::common_type(*lhs, *rhs);
      if (common == nullptr)
      {
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
        expr.result_type = lhs;
        return;
      }
      require_type(context, expr.operands[0], *common);
      require_type(context, expr.operands[1], *common);
      expr.result_type = common;
      return;
    }
    case Operator::ASSIGN:
      require_type(context, expr.operands[1], *lhs);
      return;
    }
    //log(context, "cannot calculate result type for operator" + expr.operation->token);
    return;
  }
  if (!expr.operands.empty())
  {
    expr.result_type = entities::get_type(expr.operands.back());
  }
}
void rychkov::CParser::require_type(CParseContext& context, entities::Expression::operand& expr,
    const typing::Type& type)
{
  const typing::Type* from = entities::get_type(expr);
  switch (typing::check_cast(type, *from))
  {
  case typing::NO_CAST:
    start_log(context) << "cannot cast " << *from << " to " << type;
    finish_log(context);
    break;
  case typing::IMPLICIT:
    entities::CastOperation temp = {type, false, entities::Expression{nullptr, {std::move(expr)}}};
    expr = std::move(temp);
    break;
  }
}
