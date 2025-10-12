#include "cparser.hpp"

void rychkov::CParser::append(CParseContext& context, const std::vector< rychkov::Operator >& cases)
{
  if (!type_parser_.empty())
  {
    if (cases[0].token == "*")
    {
      return append(context, '*');
    }
    if (!flush_type_parser(context))
    {
      log(context, "operator" + cases[0].token + " cannot be parsed here");
      return;
    }
  }

  if (global_scope())
  {
    log(context, "operators cannot be in global scope");
    return;
  }

  if ((cases.size() == 1) && (cases[0].category == Operator::ASSIGN))
  {
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
      if (boost::variant2::holds_alternative< entities::Variable >(decl.data))
      {
        decl.value = entities::Expression{};
        stack_.push(&*decl.value);
        return;
      }
      log(context, "unexpected assign");
      return;
    }
  }

  bool result = false;
  if (cases.size() == 2)
  {
    if (cases[0].type == Operator::BINARY)
    {
      result = parse_binary(context, cases[0]) || parse_unary(context, cases[1]);
    }
    else if (cases[1].type == Operator::BINARY)
    {
      result = parse_binary(context, cases[1]) || parse_unary(context, cases[0]);
    }
    else
    {
      result = parse_unary(context, cases[0]) || parse_unary(context, cases[1]);
    }
  }
  else if (cases[0].type == Operator::BINARY)
  {
    result = parse_binary(context, cases[0]);
  }
  else
  {
    result = parse_unary(context, cases[0]);
  }
  if (!result)
  {
    log(context, "cannot parse operator" + cases[0].token + " here");
  }
}
bool rychkov::CParser::parse_binary(CParseContext& context, const rychkov::Operator& oper)
{
  if (!stack_.top()->full())
  {
    return false;
  }
  fold(context, &oper);
  return true;
}
bool rychkov::CParser::parse_unary(CParseContext& context, const rychkov::Operator& oper)
{
  if (oper.right_align)
  {
    if ((!stack_.top()->empty() && entities::is_bridge(*stack_.top())) || stack_.top()->full())
    {
      return false;
    }
    entities::Expression* temp = new entities::Expression{&oper, {}};
    stack_.top()->operands.push_back(temp);
    stack_.push(temp);
  }
  else
  {
    if (!stack_.top()->full())
    {
      return false;
    }
    move_down();
    stack_.top()->operation = &oper;
    calculate_type(context, *stack_.top());
    stack_.pop();
  }
  return true;
}
