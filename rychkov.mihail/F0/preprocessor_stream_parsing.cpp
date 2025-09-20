#include "preprocessor.hpp"

#include <iostream>
#include <utility>
#include <algorithm>

void rychkov::Preprocessor::append(CParseContext& context, char c)
{
  if (screened_ && (c == '\n'))
  {
    screened_ = false;
    return;
  }
  if (!screened_ && (c == '\\'))
  {
    screened_ = true;
    return;
  }
  switch (state_)
  {
  case SINGLE_LINE_COMMENT:
    if (c == '\n')
    {
      state_ = prev_state_;
      prev_state_ = NO_STATE;
      screened_ = false;
      append(context, '\n');
      return;
    }
    return;
  case MULTI_LINE_COMMENT:
    if ((prev_ == '*') && (c == '/'))
    {
      state_ = prev_state_;
      prev_state_ = NO_STATE;
      prev_ = '\0';
      screened_ = false;
      return;
    }
    prev_ = c;
    return;
  case STRING_LITERAL:
    if (screened_)
    {
      buf_ += '\\';
      buf_ += c;
      screened_ = false;
      return;
    }
    if (c == '\n')
    {
      log(context, "string literal cannot contain multiple lines");
      return;
    }
    buf_ += c;
    if (c == '"')
    {
      flush_buf(context);
    }
    return;
  case CHAR_LITERAL:
    if (screened_)
    {
      buf_ += '\\';
      buf_ += c;
      screened_ = false;
      return;
    }
    if (c == '\n')
    {
      log(context, "char literal cannot contain multiple lines");
      return;
    }
    buf_ += c;
    if (c == '\'')
    {
      flush_buf(context);
    }
    return;
  case NAME:
    if (screened_)
    {
      flush_buf(context);
      append(context, '\\');
      append(context, c);
      screened_ = false;
      return;
    }
    if (!std::isalnum(c) && (c != '_'))
    {
      flush_buf(context);
      append(context, c);
      return;
    }
    buf_ += c;
    return;
  case NUMBER:
    if (screened_)
    {
      flush_buf(context);
      append(context, '\\');
      append(context, c);
      screened_ = false;
      return;
    }
    if (!std::isalnum(c) && (c != '_') && (c != '.') && (c != '\''))
    {
      flush_buf(context);
      append(context, c);
      return;
    }
    buf_ += c;
    return;
  case MACRO_PARAMETERS:
  case DIRECTIVE:
  case NO_STATE:
    if (screened_)
    {
      flush(context, '\\');
      screened_ = false;
      empty_line_ = false;
    }
    if (prev_ == '/')
    {
      if (c == '/')
      {
        prev_state_ = state_;
        state_ = SINGLE_LINE_COMMENT;
        prev_ = '\0';
        return;
      }
      else if (c == '*')
      {
        prev_state_ = state_;
        state_ = MULTI_LINE_COMMENT;
        prev_ = '\0';
        return;
      }
      flush(context, '/');
      empty_line_ = false;
      prev_ = '\0';
    }
    if (c == '/')
    {
      prev_ = '/';
      return;
    }
    if (empty_line_ && (c == '#'))
    {
      prev_state_ = NO_STATE;
      state_ = DIRECTIVE;
      return;
    }
    if (empty_line_ && !std::isspace(c))
    {
      empty_line_ = false;
    }
    if (state_ == MACRO_PARAMETERS)
    {
      if (c == '(')
      {
        parentheses_depth_++;
        if (parentheses_depth_ == 1)
        {
          return;
        }
      }
      if (c == ')')
      {
        if (parentheses_depth_ == 0)
        {
          log(context, "function-style macro must have call list");
          return;
        }
        parentheses_depth_--;
        if (parentheses_depth_ == 0)
        {
          state_ = prev_state_;
          prev_state_ = NO_STATE;
          expanse_macro(context);
          flush_buf(context);
          return;
        }
      }
      if ((c == ',') && (parentheses_depth_ == 1))
      {
        remove_whitespaces(buf_);
        expansion_list_.push_back(std::move(buf_));
        buf_.clear();
        return;
      }
      if (!std::isspace(c) && (parentheses_depth_ == 0))
      {
        log(context, "function-style macro must have call list");
        return;
      }
      if ((parentheses_depth_ != 0) && (c == '\n'))
      {
        log(context, "function-style macro parameters cannot contain multiple lines");
        return;
      }
    }
    else if (state_ != DIRECTIVE)
    {
      if (std::isalpha(c) || (c == '_'))
      {
        buf_ += c;
        state_ = NAME;
        return;
      }
      if (std::isdigit(c))
      {
        buf_ += c;
        state_ = NUMBER;
        return;
      }
    }
    if (c == '"')
    {
      buf_ += '"';
      prev_state_ = state_;
      state_ = STRING_LITERAL;
      return;
    }
    if (c == '\'')
    {
      buf_ += '\'';
      prev_state_ = state_;
      state_ = CHAR_LITERAL;
      return;
    }
    if (c == '\n')
    {
      if (state_ == DIRECTIVE)
      {
        flush_buf(context);
        append(context, '\n');
        return;
      }
      empty_line_ = true;
    }
    flush(context, c);
    return;
  }
}
