#include "cparser.hpp"

#include <iostream>
#include "print_content.hpp"

bool rychkov::CParser::parse_semicolon(CParseContext& context)
{
  bool not_first = false;
  while (!stack_.empty() && !entities::is_body(*stack_.top()) && !entities::is_decl(*stack_.top()))
  {
    if (not_first)
    {
      log(context, "found not paired closed [] or () pair");
      not_first = true;
    }
    fold(context, nullptr);
    calculate_type(context, *stack_.top());
    stack_.pop();
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
      base_types_.insert(std::make_pair(typing::Type{data.name, typing::STRUCT}, stack_.size()));
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
      if ((decl.value != nullptr) && (decl.value->result_type != nullptr))
      {
        switch (typing::check_cast(data.type, *decl.value->result_type))
        {
        case typing::NO_CAST:
          start_log(context) << "variable " << data.name << " cannot be initialized with " << *decl.value->result_type;
          finish_log(context);
          break;
        case typing::IMPLICIT:
          entities::CastOperation temp = {data.type, false, std::move(decl.value)};
          decl.value = std::move(temp);
          break;
        }
      }
      //if (last_empty)
      //{
        //log(context, "variable definition wasn't finished");
      //}
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
