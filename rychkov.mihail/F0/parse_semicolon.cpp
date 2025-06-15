#include "code_parsers.hpp"

bool rychkov::CParser::parse_semicolon(CParseContext& context)
{
  bool last_empty = false;
  for (; !stack_.empty() && !entities::is_body(*stack_.top()) && !entities::is_decl(*stack_.top()); stack_.pop())
  {
    if (!stack_.top()->empty() && !stack_.top()->full())
    {
      log(context, "found not full expression during semicolon folding");
    }
    last_empty = stack_.top()->empty();
  }
  if (stack_.empty())
  {
    return append_empty(context);
  }
  if (entities::is_body(*stack_.top())) //append new empty to body
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data.emplace_back());
    return true;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
    if (std::holds_alternative< entities::Struct >(decl.data))
    {
      entities::Struct& data = std::get< entities::Struct >(decl.data);
      if (data.name.empty())
      {
        log(context, "struct must have name");
        return false;
      }
      stack_.pop();
      structs_.insert(std::make_pair(data, stack_.size()));
      base_types_.insert(std::make_pair(typing::Type{data.name, typing::Type::Struct}, stack_.size()));
      return append_empty(context);
    }
    if (std::holds_alternative< entities::Variable >(decl.data))
    {
      entities::Variable& data = std::get< entities::Variable >(decl.data);
      if (data.name.empty())
      {
        log(context, "variable must have name");
        return false;
      }
      if (last_empty)
      {
        log(context, "variable definition wasn't finished");
      }
      stack_.pop();
      variables_.insert(std::make_pair(data, stack_.size()));
      return append_empty(context);
    }
    if (std::holds_alternative< entities::Function >(decl.data))
    {
      entities::Function& data = std::get< entities::Function >(decl.data);
      stack_.pop();
      variables_.insert(std::make_pair(entities::Variable{data.type, data.name}, stack_.size()));
      return append_empty(context);
    }
  }
  stack_.push(&program_.emplace_back()); //append new empty to program
  return true;
}
