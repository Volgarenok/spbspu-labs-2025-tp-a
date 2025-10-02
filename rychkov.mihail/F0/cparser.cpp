#include "cparser.hpp"

#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include "print_content.hpp"

using namespace std::literals::string_literals;

rychkov::CParser::CParser():
  program_{{}}
{
  stack_.push(&program_[0]);
}

std::vector< rychkov::entities::Expression >::const_iterator rychkov::CParser::begin() const
{
  return program_.cbegin();
}
std::vector< rychkov::entities::Expression >::const_iterator rychkov::CParser::end() const
{
  return program_.cend();
}
const rychkov::TypeParser& rychkov::CParser::next() const
{
  return type_parser_;
}
void rychkov::CParser::prepare_type()
{
  type_parser_.prepare();
}
void rychkov::CParser::clear_program()
{
  program_ = {{}};
  stack_ = {};
  stack_.push(&program_[0]);
  type_parser_.clear();
}
void rychkov::CParser::push_back(entities::Expression expr)
{
  program_.push_back(std::move(expr));
}

const rychkov::Operator rychkov::CParser::parentheses = {Operator::MULTIPLE, Operator::SPECIAL,
    "()", false, false, false, 1};
const rychkov::Operator rychkov::CParser::brackets = {Operator::BINARY, Operator::SPECIAL,
    "[]", false, false, false, 1};
const rychkov::Operator rychkov::CParser::comma = {Operator::BINARY, Operator::SPECIAL,
    ",", false, false, false, 15};
const rychkov::Operator rychkov::CParser::inline_if = {Operator::TERNARY, Operator::SPECIAL,
    "?:", false, false, false, 13};

bool rychkov::CParser::global_scope() const noexcept
{
  return (stack_.size() == 1) && (stack_.top()->empty());
}

void rychkov::CParser::append(CParseContext& context, entities::Literal literal)
{
  if (!type_parser_.empty())
  {
    if (literal.type != entities::Literal::Number)
    {
      log(context, "non-numeric literal cannot be a part of a type");
      return;
    }
    try
    {
      type_parser_.append(context, std::stoull(literal.literal, nullptr, 0));
    }
    catch (...)
    {
      log(context, "failed to parse numeric literal");
    }
    return;
  }
  if (global_scope() || stack_.top()->full())
  {
    log(context, "unexpected literal");
    return;
  }
  stack_.top()->operands.push_back(std::move(literal));
}
void rychkov::CParser::append(CParseContext& context, TypeKeyword keyword)
{
  (type_parser_.*(type_keywords.at(keyword)))(context);
}
void rychkov::CParser::append(CParseContext& context, std::string name)
{
  decltype(base_types)::const_iterator base_type_p = base_types.find(name);
  if (base_type_p != base_types.cend())
  {
    type_parser_.append(context, base_type_p->first);
    return;
  }
  decltype(aliases)::const_iterator alias_p = aliases.find(name);
  if (alias_p != aliases.cend())
  {
    type_parser_.append(context, alias_p->type);
    return;
  }
  if (!type_parser_.empty())
  {
    type_parser_.append(context, name);
    return;
  }
  decltype(variables)::const_iterator var_p = variables.find(name);
  if (var_p != variables.cend())
  {
    if (stack_.top()->full())
    {
      log(context, "unexpected variable");
      return;
    }
    stack_.top()->operands.push_back(var_p->first);
    return;
  }

  if (stack_.top()->empty())
  {
    log(context, "unexpected name (" + name + ")");
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
        data.name = std::move(name);
        return;
      }
      log(context, "struct name duplicating (" + name + ")");
      return;
    }
  }
  log(context, "unexpected name (" + name + ")");
}

void rychkov::CParser::append(CParseContext& context, char c)
{
  if (!type_parser_.empty())
  {
    if (!type_parser_.ready() || (c == '*') || (c == '(') || (c == '['))
    {
      type_parser_.append(context, c);
    }
    else if (flush_type_parser(context))
    {
      append(context, c);
    }
    return;
  }
  using append_signature = void(CParser::*)(CParseContext&);
  using append_map = std::map< char, append_signature >;
  static const append_map dispatch_map = {
        {';', &CParser::parse_semicolon},
        {'{', &CParser::parse_open_brace},
        {'}', &CParser::parse_close_brace},
        {'(', &CParser::parse_open_parenthesis},
        {')', &CParser::parse_close_parenthesis},
        {'[', &CParser::parse_open_bracket},
        {']', &CParser::parse_close_bracket},
        {',', &CParser::parse_comma},
        {'?', &CParser::parse_question_mark},
        {':', &CParser::parse_colon}
      };

  append_map::const_iterator found = dispatch_map.find(c);
  if (found != dispatch_map.cend())
  {
    (this->*(found->second))(context);
    return;
  }
  log(context, "unknown symbol ('"s + c + "'; code = " + std::to_string(c) + ")");
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
    type_parser_.prepare();
    if (entities::is_decl(*stack_.top()))
    {
      entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
      if (boost::variant2::holds_alternative< entities::Alias >(decl.data))
      {
        entities::Alias& alias = boost::variant2::get< entities::Alias >(decl.data);
        if (!alias.name.empty())
        {
          log(context, "duplicating types in typedef declaration");
        }
        else if (type_parser_.is_type())
        {
          log(context, "missing alias name");
        }
        else
        {
          entities::Variable temp = type_parser_.variable();
          alias = {std::move(temp.type), std::move(temp.name)};
        }
      }
      type_parser_.clear();
      return true;
    }
    if (type_parser_.is_function())
    {
      if (!global_scope())
      {
        log(context, "functions can be declared only in global scope");
        type_parser_.clear();
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.function()};
      variables.insert({type_parser_.variable(), stack_.size()});
    }
    else
    {
      if (!stack_.top()->empty())
      {
        log(context, "variable declaration cannot be placed here");
        type_parser_.clear();
        return false;
      }
      if (variables.find({{{}, type_parser_.name()}, stack_.size()}) != variables.end())
      {
        log(context, "variable name repeats in one scope");
        type_parser_.clear();
        return false;
      }
      *stack_.top() = entities::Declaration{type_parser_.variable()};
      variables.insert({type_parser_.variable(), stack_.size()});
    }
  }
  type_parser_.clear();
  return true;
}
bool rychkov::CParser::append_empty(CParseContext& context)
{
  if (stack_.empty())
  {
    program_.emplace_back();
    stack_.push(&program_.back());
    return true;
  }
  if (entities::is_body(*stack_.top()))
  {
    entities::Body& body = boost::variant2::get< entities::Body >(stack_.top()->operands[0]);
    body.data.emplace_back();
    stack_.push(&body.data.back());
    return true;
  }
  if (entities::is_decl(*stack_.top()))
  {
    entities::Declaration& decl = boost::variant2::get< entities::Declaration >(stack_.top()->operands[0]);
    if (decl.value != nullptr)
    {
      stack_.pop();
      return append_empty(context);
    }
    return true;
  }
  log(context, "cannot append any expr to this place");
  return false;
}
