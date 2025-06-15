#include "cparser.hpp"

#include <iostream>
#include <map>
#include "print_content.hpp"

using namespace std::literals::string_literals;

const std::vector< rychkov::Operator > rychkov::CParser::operators = {
      {rychkov::Operator::unary, rychkov::Operator::special, "sizeof", false, true, 2}
    };

rychkov::CParser::CParser():
  program_{{}},
  base_types_{
        {{"int", typing::Type::Int}, 0},
        {{"char", typing::Type::Int}, 0}
      }
{
  stack_.push(&program_[0]);
}

void rychkov::CParser::print(std::ostream& out) const
{
  ContentPrinter printer{out};
  for (const entities::Expression& i: program_)
  {
    printer(i);
  }
}
bool rychkov::CParser::global_scope() const noexcept
{
  return (stack_.size() == 1) && (stack_.top()->empty());
}

bool rychkov::CParser::append(CParseContext& context, entities::Literal literal)
{
  if (!type_parser_.empty())
  {
    if (literal.type != entities::Literal::Number)
    {
      log(context, "non-numeric literal cannot be a part of a type");
      return false;
    }
    try
    {
      return type_parser_.append(context, std::stoull(literal.literal, nullptr, 0));
    }
    catch (...)
    {
      return false;
    }
  }
  if (global_scope() || stack_.top()->full())
  {
    log(context, "unexpected literal");
    return false;
  }
  stack_.top()->operands.push_back(std::move(literal));
  return true;
}
bool rychkov::CParser::append(CParseContext& context, std::string name)
{
  decltype(base_types_)::const_iterator base_type_p = base_types_.find(name);
  if (base_type_p != base_types_.cend())
  {
    return type_parser_.append(context, base_type_p->first);
  }
  if (!type_parser_.empty())
  {
    return type_parser_.append(context, name);
  }
  static const std::set< std::string > type_keywords = {"const", "volatile", "signed", "unsigned"};
  if (type_keywords.find(name) != type_keywords.end())
  {
    return type_parser_.append(context, name);
  }

  decltype(variables_)::const_iterator var_p = variables_.find(name);
  if (var_p != variables_.cend())
  {
    if (stack_.top()->full())
    {
      log(context, "unexpected variable");
      return false;
    }
    stack_.top()->operands.push_back(var_p->first);
    return true;
  }

  if (name == "struct")
  {
    if (global_scope() || stack_.top()->empty())
    {
      stack_.top()->operands.push_back(entities::Declaration{entities::Struct{}});
      return true;
    }
    log(context, "struct can't be declared in this context");
    return false;
  }

  if (stack_.top()->empty())
  {
    log(context, "unexpected name (" + name + ")");
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
        data.name = std::move(name);
        return true;
      }
      log(context, "struct name duplicating (" + name + ")");
      return false;
    }
  }
  return true;
}
bool rychkov::CParser::append(CParseContext& context, char c)
{
  if (!type_parser_.empty())
  {
    if (!type_parser_.ready() || (c == '*') || (c == '(') || (c == '['))
    {
      return type_parser_.append(context, c);
    }
    return flush_type_parser(context) && append(context, c);
  }
  using append_signature = bool(CParser::*)(CParseContext&);
  using append_map = std::map< char, append_signature >;
  static const append_map dispatch_map = {
        {';', &CParser::parse_semicolon},
        {'{', &CParser::parse_open_brace},
        {'}', &CParser::parse_close_brace}
      };

  append_map::const_iterator found = dispatch_map.find(c);
  if (found != dispatch_map.cend())
  {
    return (this->*(found->second))(context);
  }
  log(context, "unknown symbol ('"s + c + "'; code = " + std::to_string(c) + ")");
  return false;
}
bool rychkov::CParser::flush_type_parser(CParseContext& context)
{
  if (!type_parser_.empty())
  {
    if (!type_parser_.ready())
    {
      log(context, "type cannot be finished here");
      return false;
    }
    // if typedef
    if (type_parser_.is_function())
    {
      if (!global_scope())
      {
        log(context, "functions can be declared only in global scope");
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.function()};
      variables_.insert({type_parser_.variable(), stack_.size()});
      defined_functions_.insert(type_parser_.variable());
    }
    else
    {
      if (!stack_.top()->empty())
      {
        log(context, "variable declaration cannot be placed here");
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.variable()};
      variables_.insert({type_parser_.variable(), stack_.size()});
    }
  }
  type_parser_.clear();
  return true;
}
bool rychkov::CParser::append_empty(CParseContext& context)
{
  if (stack_.empty())
  {
    stack_.push(&program_.emplace_back());
    return true;
  }
  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = std::get< entities::Body >(stack_.top()->operands[0]);
    stack_.push(&body.data.emplace_back());
    return true;
  }
  log(context, "cannot append any expr to this place");
  return false;
}
