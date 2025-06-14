#include "code_parsers.hpp"

bool rychkov::CParser::append(CParseContext& context, const std::vector< rychkov::Operator >& cases)
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
      return false;
    }
  }

  if (global_scope())
  {
    log(context, "operators cannot be in global scope");
    return false;
  }

  if ((cases.size() == 1) && (cases[0].category == Operator::assign))
  {
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
      if (std::holds_alternative< entities::Variable >(decl.data))
      {
        entities::Variable& data = std::get< entities::Variable >(decl.data);
        decl.value = entities::Expression{};
        stack_.push(&*decl.value);
        return true;
      }
      log(context, "unexpected assign");
      return false;
    }
  }

  bool result = false;
  if (cases.size() == 2)
  {
    if (cases[0].type == Operator::binary)
    {
      result = parse_binary(context, cases[0]) || parse_unary(context, cases[1]);
    }
    else if (cases[1].type == Operator::binary)
    {
      result = parse_binary(context, cases[1]) || parse_unary(context, cases[0]);
    }
    else
    {
      result = parse_unary(context, cases[0]) || parse_unary(context, cases[1]);
    }
  }
  else if (cases[0].type == Operator::binary)
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
    return false;
  }
  return true;
}
bool rychkov::CParser::parse_binary(CParseContext& context, const rychkov::Operator& oper)
{
  if (stack_.top()->operation != nullptr)
  {
    entities::Expression* last = stack_.top();
    while (stack_.top()->operation != nullptr)
    {
      if (!stack_.top()->full())
      {
        return false;
      }
      if (stack_.top()->operation->type == Operator::binary)
      {
        if ((stack_.top()->operation->priority < oper.priority)
            || ((stack_.top()->operation->priority == oper.priority) && !oper.right_align))
        {
          last = stack_.top();
          stack_.pop();
          continue;
        }
        move_down();
        stack_.top()->operation = &oper;
        return true;
      }
      else if (stack_.top()->operation->type == Operator::unary)
      {
        if ((stack_.top()->operation->priority < oper.priority)
            || ((stack_.top()->operation->priority == oper.priority) && !oper.right_align))
        {
          last = stack_.top();
          stack_.pop();
          continue;
        }
        move_down();
        stack_.top()->operation = &oper;
        return true;
      }
    }
    stack_.push(last);
    move_up();
    stack_.top()->operation = &oper;
    return true;
  }
  if (!stack_.top()->empty() && !entities::is_decl(*stack_.top()) && !entities::is_body(*stack_.top()))
  {
    move_up();
    stack_.top()->operation = &oper;
    return true;
  }
  return false;
}
bool rychkov::CParser::parse_unary(CParseContext& context, const rychkov::Operator& oper)
{
  if (oper.right_align)
  {
    if ((!stack_.top()->empty() && (stack_.top()->operation == nullptr)) || stack_.top()->full())
    {
      return false;
    }
    entities::Expression* temp = new entities::Expression{&oper, {}, {}};
    stack_.top()->operands.push_back(temp);
    stack_.push(temp);
    return true;
  }
  else
  {
    if (!stack_.top()->full())
    {
      return false;
    }
    entities::Expression* temp = new entities::Expression{&oper, {}, {std::move(stack_.top()->operands.back())}};
    stack_.top()->operands.back() = temp;
    temp->operation = &oper;
    return true;
  }
}
rychkov::entities::Expression* rychkov::CParser::move_up()
{
  entities::Expression* old = new entities::Expression{std::move(*stack_.top())};
  *stack_.top() = {nullptr, {}, {old}};
  return old;
}
void rychkov::CParser::move_down()
{
  entities::Expression* temp = new entities::Expression{nullptr, {}, {std::move(stack_.top()->operands.back())}};
  stack_.top()->operands.back() = temp;
  stack_.push(temp);
}
