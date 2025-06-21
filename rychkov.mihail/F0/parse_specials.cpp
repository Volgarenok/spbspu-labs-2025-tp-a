#include "cparser.hpp"

bool rychkov::CParser::parse_open_parenthesis(CParseContext& context)
{
  if (stack_.top()->full())
  {
    move_down(context);
    stack_.top()->operation = &parentheses;
  }
  entities::Expression* temp = new entities::Expression;
  stack_.top()->operands.push_back(temp);
  stack_.push(temp);
  return true;
}
bool rychkov::CParser::parse_close_parenthesis(CParseContext& context)
{
  fold(context, nullptr);
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (entities::is_decl(*stack_.top()) || entities::is_body(*stack_.top()))
  {
    log(context, "found not paired ')'");
    stack_.push(last);
    return false;
  }
  remove_bridge(*last);
  calculate_type(context, *last);
  if (stack_.top()->operation == &parentheses)
  {
    calculate_type(context, *stack_.top());
    stack_.pop();
  }
  return true;
}
bool rychkov::CParser::parse_open_bracket(CParseContext& context)
{
  if (!stack_.top()->full())
  {
    log(context, "operator[] cannot be applied here");
    return false;
  }
  move_down(context);
  stack_.top()->operation = &brackets;
  entities::Expression* temp = new entities::Expression;
  stack_.top()->operands.push_back(temp);
  stack_.push(temp);
  return true;
}
bool rychkov::CParser::parse_close_bracket(CParseContext& context)
{
  fold(context, nullptr);
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (stack_.top()->operation != &brackets)
  {
    log(context, "found not paired ']'");
    stack_.push(last);
    return false;
  }
  remove_bridge(*last);
  calculate_type(context, *last);
  calculate_type(context, *stack_.top());
  stack_.pop();
  return true;
}
bool rychkov::CParser::parse_comma(CParseContext& context)
{
  fold(context, nullptr);
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (stack_.top()->operation == &parentheses)
  {
    remove_bridge(*last);
    entities::Expression* temp = new entities::Expression{};
    stack_.top()->operands.push_back(temp);
    stack_.push(temp);
    return true;
  }
  stack_.push(last);
  if (entities::is_bridge(*last))
  {
    stack_.push(move_up());
    stack_.top()->operation = &comma;
    return true;
  }
  move_up();
  stack_.top()->operation = &comma;
  return true;
}
