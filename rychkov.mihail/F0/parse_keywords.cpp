#include "cparser.hpp"

void rychkov::CParser::parse_typedef(CParseContext& context)
{
  if (global_scope())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Alias{}});
    return;
  }
  log(context, "typedef cannot be declared in this scope");
  return;
}
void rychkov::CParser::parse_struct(CParseContext& context)
{
  if (global_scope() || stack_.top()->empty())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Struct{}});
    return;
  }
  log(context, "struct cannot be declared in this context");
  return;
}
void rychkov::CParser::parse_union(CParseContext& context)
{
  if (global_scope() || stack_.top()->empty())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Union{}});
    return;
  }
  log(context, "union cannot be declared in this context");
  return;
}
void rychkov::CParser::parse_enum(CParseContext& context)
{
  if (global_scope() || stack_.top()->empty())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Enum{}});
    return;
  }
  log(context, "enum cannot be declared in this context");
  return;
}
void rychkov::CParser::parse_return(CParseContext& context)
{
  if (stack_.top()->empty())
  {
    entities::Expression* temp = new entities::Expression;
    stack_.top()->operands.push_back(entities::Declaration{entities::Statement{entities::Statement::RETURN}, temp});
    stack_.push(temp);
    return;
  }
  log(context, "return cannot be declared in this context");
  return;
}
void rychkov::CParser::parse_if(CParseContext& context)
{
  check_statement_placement(context);
  if (stack_.top()->empty())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Statement{entities::Statement::IF}});
    return;
  }
  log(context, "if cannot be declared in this context");
  return;
}
void rychkov::CParser::parse_while(CParseContext& context)
{
  check_statement_placement(context);
  if (stack_.top()->empty())
  {
    stack_.top()->operands.push_back(entities::Declaration{entities::Statement{entities::Statement::WHILE}});
    return;
  }
  log(context, "while cannot be declared in this context");
  return;
}
void rychkov::CParser::check_statement_placement(CParseContext& context)
{
  entities::Expression* last = stack_.top();
  stack_.pop();
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
    if ((last != &*decl.value) || !boost::variant2::holds_alternative< entities::Statement >(decl.data))
    {
      log(context, "cannot place statement here");
    }
    else
    {
      entities::Statement& statement = boost::variant2::get< entities::Statement >(decl.data);
      if (statement.type == entities::Statement::RETURN)
      {
        log(context, "cannot place statement as return result");
      }
    }
  }
  stack_.push(last);
}
