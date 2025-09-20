#include "cparser.hpp"

#include <iostream>
#include "print_content.hpp"

void rychkov::CParser::parse_semicolon(CParseContext& context)
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
    entities::remove_bridge(*stack_.top());
    calculate_type(context, *stack_.top());
    stack_.pop();
  }
  if (stack_.empty())
  {
    append_empty(context);
    return;
  }

  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = boost::variant2::get< entities::Body >(stack_.top()->operands[0]);
    body.data.emplace_back();
    stack_.push(&body.data.back());
    return;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
    if (boost::variant2::holds_alternative< entities::Struct >(decl.data))
    {
      entities::Struct& data = boost::variant2::get< entities::Struct >(decl.data);
      if (data.name.empty())
      {
        log(context, "struct must have name");
        return;
      }
      stack_.pop();
      structs.insert(std::make_pair(data, stack_.size()));
      base_types.insert(std::make_pair(typing::Type{data.name, typing::STRUCT}, stack_.size()));
      append_empty(context);
      return;
    }
    if (boost::variant2::holds_alternative< entities::Variable >(decl.data))
    {
      entities::Variable& data = boost::variant2::get< entities::Variable >(decl.data);
      if (data.name.empty())
      {
        log(context, "variable must have name");
        return;
      }
      if (decl.value != nullptr)
      {
        require_type(context, *decl.value, data.type);
      }
      if (typing::is_array(&data.type) && !data.type.array_has_length)
      {
        if (decl.value != nullptr)
        {
          if (!decl.value->result_type.array_has_length)
          {
            log(context, "cannot deduce array length");
          }
          else
          {
            data.type.array_has_length = true;
            data.type.array_length = decl.value->result_type.array_length;
          }
        }
      }
      stack_.pop();
      append_empty(context);
      return;
    }
    if (boost::variant2::holds_alternative< entities::Function >(decl.data))
    {
      stack_.pop();
      append_empty(context);
      return;
    }
    if (boost::variant2::holds_alternative< entities::Alias >(decl.data))
    {
      entities::Alias& data = boost::variant2::get< entities::Alias >(decl.data);
      if (data.name.empty())
      {
        log(context, "missing alias name");
      }
      else
      {
        aliases.insert(data);
      }
      stack_.pop();
      append_empty(context);
      return;
    }
    if (boost::variant2::holds_alternative< entities::Statement >(decl.data))
    {
      entities::Statement& data = boost::variant2::get< entities::Statement >(decl.data);
      if ((data.type == entities::Statement::RETURN) || (data.type == entities::Statement::IF)
          || (data.type == entities::Statement::WHILE))
      {
        stack_.pop();
        append_empty(context);
        clear_scope();
        return;
      }
    }
    log(context, "cannot parse ';' here");
  }
  program_.emplace_back();
  stack_.push(&program_.back());
}
