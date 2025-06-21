#include "preprocessor.hpp"

#include <iostream>
#include <sstream>
#include <cctype>
#include <utility>
#include "lexer.hpp"

rychkov::Parser::map_type< rychkov::Preprocessor::CommandContext, rychkov::Preprocessor >
    rychkov::Preprocessor::call_map = {
      {"include", &rychkov::Preprocessor::include},
      {"define", &rychkov::Preprocessor::define},
      {"pragma", &rychkov::Preprocessor::pragma},
      {"undef", &rychkov::Preprocessor::undef},
      {"ifdef", &rychkov::Preprocessor::ifdef},
      {"ifndef", &rychkov::Preprocessor::ifndef},
      {"else", &rychkov::Preprocessor::else_cmd},
      {"endif", &rychkov::Preprocessor::endif}
    };

rychkov::Preprocessor::Preprocessor():
  next_{nullptr}
{}
rychkov::Preprocessor::Preprocessor(Lexer* next, std::vector< std::string > search_dirs):
  next_{next},
  include_dirs_(std::move(search_dirs))
{}

std::string rychkov::Preprocessor::get_name(std::istream& in)
{
  std::string name;
  for (; in && std::isalnum(in.peek()) || (in.peek() == '_'); name += in.get())
  {}
  return std::move(name);
}
void rychkov::Preprocessor::remove_whitespaces(std::string& str)
{
  std::string::size_type pos = str.find_last_not_of(" \t\v\n\r");
  if (pos != std::string::npos)
  {
    str.erase(pos);
  }
  pos = str.find_first_not_of(" \t\v\n\r");
  if (pos != std::string::npos)
  {
    str.erase(0, pos);
  }
}
void rychkov::Preprocessor::CommandContext::parse_error()
{
  in.setstate(std::ios::failbit);
}
void rychkov::Preprocessor::flush(CParseContext& context, char c)
{
  if (next_ == nullptr)
  {
    context.out << c;
  }
  else
  {
    next_->append(context, c);
  }
}
void rychkov::Preprocessor::flush_buf(CParseContext& context)
{
  if (directive_)
  {
    directive_ = false;
    std::istringstream in(buf_);
    CommandContext cmd_context{in >> std::noskipws, context.err, context};
    rychkov::Parser::parse(cmd_context, *this, rychkov::Preprocessor::call_map);
    if (!in)
    {
      log(context, "unknown preprocessor command");
    }
    buf_.clear();
    return;
  }
  if (!conditional_pairs_.empty() && ((conditional_pairs_.top() == WAIT_ELSE)
      || (conditional_pairs_.top() == SKIP_ELSE)))
  {
    return;
  }
  if (expansion_ != nullptr)
  {
    log(context, "expansion of macro hasn't finished - aborting");
    expansion_ = nullptr;
    expansion_list_.clear();
    return;
  }
  decltype(macros_)::iterator macro_p = macros_.find(buf_);
  if (macro_p != macros_.end())
  {
    if (macro_p->func_style)
    {
      expansion_ = &*macro_p;
      parentheses_depth_ = 0;
      buf_.clear();
      return;
    }
    expansion_ = &*macro_p;
    expanse_macro(context);
    return;
  }
  for (char c: buf_)
  {
    flush(context, c);
  }
  buf_.clear();
}
void rychkov::Preprocessor::flush(CParseContext& context)
{
  flush_buf(context);
  if (next_ != nullptr)
  {
    next_->flush(context);
  }
  if (!conditional_pairs_.empty())
  {
    log(context, "not all #if-#else-#endif were closed");
  }
}
void rychkov::Preprocessor::append(CParseContext& context, char c)
{
  bool skip = !conditional_pairs_.empty() && ((conditional_pairs_.top() == WAIT_ELSE)
    || (conditional_pairs_.top() == SKIP_ELSE));

  if (empty_line_ && (c == '#'))
  {
    directive_ = true;
    empty_line_ = false;
    return;
  }
  if (c == '\n')
  {
    flush_buf(context);
    if (!skip)
    {
      flush(context, '\n');
    }
    empty_line_ = true;
    return;
  }
  if (skip)
  {
    if (directive_)
    {
      buf_ += c;
    }
    if (!std::isspace(c))
    {
      empty_line_ = false;
    }
    return;
  }

  if (c == '(')
  {
    parentheses_depth_++;
    if ((expansion_ != nullptr) && (parentheses_depth_ == 1))
    {
      return;
    }
  }
  if ((c == ')') && (expansion_ != nullptr))
  {
    if (parentheses_depth_ == 0)
    {
      log(context, "function-style macro must have call list");
      return;
    }
    parentheses_depth_--;
    if (parentheses_depth_ == 0)
    {
      expanse_macro(context);
      return;
    }
  }
  if ((c == ',') && (expansion_ != nullptr) && (parentheses_depth_ == 1))
  {
    remove_whitespaces(buf_);
    expansion_list_.push_back(std::move(buf_));
    buf_.clear();
    return;
  }
  if (!std::isspace(c))
  {
    empty_line_ = false;
    if ((expansion_ != nullptr) && (parentheses_depth_ == 0))
    {
      log(context, "function-style macro must have call list");
      return;
    }
  }
  if (std::isalnum(c) || (c == '_') || directive_ || (expansion_ != nullptr))
  {
    buf_ += c;
    return;
  }
  flush_buf(context);
  if (expansion_ != nullptr)
  {
    append(context, c);
    return;
  }
  flush(context, c);
}
void rychkov::Preprocessor::expanse_macro(CParseContext& context)
{
  const Macro* macro_p = std::exchange(expansion_, nullptr);
  std::string body = macro_p->body;
  if (macro_p->func_style)
  {
    remove_whitespaces(buf_);
    if (!buf_.empty())
    {
      expansion_list_.push_back(std::move(buf_));
    }
    if (expansion_list_.size() > macro_p->parameters.size())
    {
      log(context, "too many arguments for expansion of macro " + macro_p->name);
      expansion_list_.resize(macro_p->parameters.size());
    }
    for (size_t i = 0; i < expansion_list_.size(); i++)
    {
      std::string::size_type pos = body.find(macro_p->parameters[i]);
      while (pos != std::string::npos)
      {
        body.replace(pos, macro_p->parameters[i].length(), expansion_list_[i]);
        pos = body.find(macro_p->parameters[i]);
      }
    }
    std::string::size_type pos = body.find("##");
    while (pos != std::string::npos)
    {
      body.erase(pos, 2);
      pos = body.find("##");
    }
    expansion_list_.clear();
  }
  buf_.clear();
  std::istringstream in(body);
  CParseContext expanse_context = {context.out, context.err, macro_p->name, &context, true};
  parse(expanse_context, in, false);
  context.nerrors += expanse_context.nerrors;
}
