#include "code_parsers.hpp"

bool rychkov::CParser::parse_open_brace(CParseContext& context)
{
  if (global_scope())
  {
    log(context, "braced-enclosed body cannot be declared alone in global scope");
    return false;
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
      decl.value = entities::Body{};
      stack_.push(&*decl.value);
      entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
      stack_.push(&body.data[0]);
      return true;
    }
    if (std::holds_alternative< entities::Function >(decl.data))
    {
      decl.value = entities::Body{};
      stack_.push(&*decl.value);
      entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
      stack_.push(&body.data[0]);
      return true;
    }
    log(context, "braced enclosed body cannot follow this declaration");
    return false;
  }
  /*else if (entities::is_body(*stack_.top()))
  {
  entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
  stack_.push(&body.data.emplace_back());
  return true;
  }*/
  log(context, "unexpected braced-enclosed body");
  return false;
}
bool rychkov::CParser::parse_close_brace(CParseContext& context)
{
  if (stack_.size() == 1)
  {
    log(context, "unpaired closing brace");
    return false;
  }
  if (stack_.top()->empty())
  {
    stack_.pop();
  }
  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    body.data.pop_back(); // delete empty
    stack_.pop();
    // determine what was closed
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = std::get< entities::Declaration >(stack_.top()->operands[0]);
      if (std::holds_alternative< entities::Function >(decl.data))
      {
        stack_.top() = &program_.emplace_back();
      }
      // otherwise we will wait for variables
    }
    else if (entities::is_body(*stack_.top()))
    {
      entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
      stack_.push(&body.data.emplace_back());
    }
    return true;
  }
  log(context, "unpaired closing brace");
  return false;
}
