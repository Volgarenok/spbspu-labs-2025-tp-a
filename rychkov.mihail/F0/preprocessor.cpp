#include "preprocessor.hpp"

#include <iostream>
#include <sstream>
#include <cctype>
#include <utility>
#include "lexer.hpp"

rychkov::Preprocessor::Preprocessor():
  next{nullptr}
{}
rychkov::Preprocessor::Preprocessor(std::unique_ptr< Lexer > lexer, std::vector< std::string > search_dirs):
  include_paths(std::move(search_dirs)),
  next{std::move(lexer)}
{}

bool rychkov::Preprocessor::skip_all() const noexcept
{
  return !conditional_pairs_.empty() && ((conditional_pairs_.top() == WAIT_ELSE)
      || (conditional_pairs_.top() == SKIP_ELSE));
}
std::string rychkov::Preprocessor::get_name(std::istream& in)
{
  std::string name;
  for (; in && (std::isalnum(in.peek()) || (in.peek() == '_')); name += in.get())
  {}
  return name;
}
void rychkov::Preprocessor::remove_whitespaces(std::string& str)
{
  std::string::size_type pos = str.find_last_not_of(" \t\v\n\r");
  if ((pos != std::string::npos) && (pos + 1 < str.length()))
  {
    str.erase(pos + 1);
  }
  pos = str.find_first_not_of(" \t\v\n\r");
  if (pos != std::string::npos)
  {
    str.erase(0, pos);
  }
}
void rychkov::Preprocessor::flush(CParseContext& context, char c)
{
  if ((state_ == DIRECTIVE) || (state_ == MACRO_PARAMETERS))
  {
    buf_ += c;
  }
  else if (!skip_all())
  {
    if (next == nullptr)
    {
      context.out << c;
    }
    else
    {
      next->append(context, c);
    }
  }
}
void rychkov::Preprocessor::flush_buf(CParseContext& context)
{
  if (state_ == DIRECTIVE)
  {
    state_ = NO_STATE;
    prev_state_ = NO_STATE;
    std::istringstream in(buf_);
    std::string cmd;
    in >> std::noskipws >> cmd >> std::skipws;
    if (!skip_all() || (cmd == "endif") || (cmd == "else"))
    {
      decltype(directives_)::const_iterator cmd_p = directives_.find(cmd);
      if (cmd_p == directives_.cend())
      {
        log(context, "unknown preprocessor command");
      }
      else
      {
        (this->*(cmd_p->second))(in, context);
      }
    }
    buf_.clear();
    return;
  }
  if (!skip_all())
  {
    if (state_ == MACRO_PARAMETERS)
    {
      log(context, "expansion of macro hasn't finished - aborting");
      expansion_ = nullptr;
      expansion_list_.clear();
    }
    else
    {
      if (state_ == NAME)
      {
        decltype(macros)::iterator macro_p = macros.find(buf_);
        if (macro_p != macros.end())
        {
          if (macro_p->func_style)
          {
            state_ = MACRO_PARAMETERS;
            prev_state_ = NO_STATE;
            expansion_ = &*macro_p;
            parentheses_depth_ = 0;
            buf_.clear();
            return;
          }
          expansion_ = &*macro_p;
          state_ = prev_state_;
          prev_state_ = NO_STATE;
          expanse_macro(context);
          flush_buf(context);
          buf_.clear();
          return;
        }
      }
      State prev = state_;
      state_ = prev_state_;
      if ((state_ == DIRECTIVE) || (state_ == MACRO_PARAMETERS))
      {
        return;
      }
      if (!skip_all())
      {
        if (next == nullptr)
        {
          for (char c: buf_)
          {
            flush(context, c);
          }
        }
        else
        {
          switch (prev)
          {
          case rychkov::Preprocessor::STRING_LITERAL:
            next->append_string_literal(context, buf_);
            break;
          case rychkov::Preprocessor::CHAR_LITERAL:
            next->append_char_literal(context, buf_);
            break;
          case rychkov::Preprocessor::NAME:
            next->append_name(context, buf_);
            break;
          case rychkov::Preprocessor::NUMBER:
            next->append_number(context, buf_);
            break;
          default:
            for (char c: buf_)
            {
              flush(context, c);
            }
            break;
          }
        }
      }
    }
  }
  state_ = prev_state_;
  prev_state_ = NO_STATE;
  buf_.clear();
}
void rychkov::Preprocessor::flush(CParseContext& context)
{
  flush_buf(context);
  if (next != nullptr)
  {
    next->flush(context);
  }
  if (!conditional_pairs_.empty())
  {
    log(context, "not all #if-#else-#endif were closed");
  }
}
void rychkov::Preprocessor::parse(CParseContext& context, std::istream& in, bool need_flush)
{
  bool not_first_line = false;
  while (in.good())
  {
    if (not_first_line)
    {
      append(context, '\n');
      context.line++;
    }
    not_first_line = true;
    context.symbol = 0;
    std::getline(in, context.last_line);
    for (char c: context.last_line)
    {
      append(context, c);
      context.symbol++;
    }
  }
  if (need_flush)
  {
    flush(context);
  }
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
