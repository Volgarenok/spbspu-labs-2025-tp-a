#include "cparser.hpp"

void rychkov::CParser::parse_open_parenthesis(CParseContext& context)
{
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
    if (boost::variant2::holds_alternative< entities::Statement >(decl.data))
    {
      entities::Statement& statement = boost::variant2::get< entities::Statement >(decl.data);
      if ((statement.type == entities::Statement::IF) || (statement.type == entities::Statement::WHILE))
      {
        statement.conditions.emplace_back();
        stack_.push(&statement.conditions.back());
        return;
      }
    }
    log(context, "unexpected '('");
    return;
  }
  if (stack_.top()->full())
  {
    move_down();
    stack_.top()->operation = &parentheses;
  }
  entities::Expression* temp = new entities::Expression;
  stack_.top()->operands.push_back(temp);
  stack_.push(temp);
}
void rychkov::CParser::parse_close_parenthesis(CParseContext& context)
{
  fold(context, nullptr);
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
    if (boost::variant2::holds_alternative< entities::Statement >(decl.data))
    {
      entities::Statement& statement = boost::variant2::get< entities::Statement >(decl.data);
      if ((statement.type == entities::Statement::IF) || (statement.type == entities::Statement::WHILE))
      {
        if (!statement.conditions.empty() && (last == &statement.conditions.back()))
        {
          remove_bridge(*last);
          calculate_type(context, *last);
          require_type(context, statement.conditions[0], {"int", typing::BASIC});
          entities::Expression* temp = new entities::Expression;
          decl.value = temp;
          stack_.push(temp);
          return;
        }
      }
    }
    log(context, "found not paired ')'");
    stack_.push(last);
    return;
  }
  if (entities::is_body(*stack_.top()))
  {
    log(context, "found not paired ')'");
    stack_.push(last);
    return;
  }
  remove_bridge(*last);
  calculate_type(context, *last);
  if (stack_.top()->operation == &parentheses)
  {
    calculate_type(context, *stack_.top());
    stack_.pop();
  }
}
void rychkov::CParser::parse_open_bracket(CParseContext& context)
{
  if (!stack_.top()->full())
  {
    log(context, "operator[] cannot be applied here");
    return;
  }
  move_down();
  stack_.top()->operation = &brackets;
  entities::Expression* temp = new entities::Expression;
  stack_.top()->operands.push_back(temp);
  stack_.push(temp);
}
void rychkov::CParser::parse_close_bracket(CParseContext& context)
{
  fold(context, nullptr);
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (stack_.top()->operation != &brackets)
  {
    log(context, "found not paired ']'");
    stack_.push(last);
    return;
  }
  remove_bridge(*last);
  calculate_type(context, *last);
  calculate_type(context, *stack_.top());
  stack_.pop();
}
void rychkov::CParser::parse_comma(CParseContext& context)
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
    return;
  }
  stack_.push(last);
  if (entities::is_bridge(*last))
  {
    move_up_down();
    stack_.top()->operation = &comma;
    return;
  }
  move_up();
  stack_.top()->operation = &comma;
}
void rychkov::CParser::parse_question_mark(CParseContext& context)
{
  fold(context, &inline_if);
  stack_.top()->operation = &inline_if;
}
void rychkov::CParser::parse_colon(CParseContext& context)
{
  fold(context, &inline_if);
}
