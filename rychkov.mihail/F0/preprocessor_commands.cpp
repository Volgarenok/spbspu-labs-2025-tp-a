#include "preprocessor.hpp"

#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>

bool rychkov::Preprocessor::include(CommandContext& context)
{
  char quote = '\0';
  if (!(context.in >> std::ws >> quote))
  {
    log(context.parse_context, "no filename");
    return true;
  }
  std::string filename;
  if ((quote != '"') && (quote != '<'))
  {
    log(context.parse_context, "wrong quotes");
    return true;
  }
  std::getline(context.in, filename, (quote == '"' ? '"' : '>'));
  if (context.in.eof())
  {
    log(context.parse_context, "missing end quote");
    return true;
  }
  if (!(context.in >> std::ws).eof())
  {
    log(context.parse_context, "text after filename");
    return true;
  }

  buf_.clear();
  std::ifstream in;
  if (quote == '"')
  {
    in.open(filename);
  }
  else
  {
    for (const std::string& base_path: include_dirs_)
    {
      in.open(base_path + '/' + filename);
      if (in.is_open())
      {
        filename = base_path + '/' + filename;
        break;
      }
    }
  }
  if (!in)
  {
    log(context.parse_context, "failed to open file");
    return true;
  }
  CParseContext file_context = {context.parse_context.out, context.err, filename, &context.parse_context};
  parse(file_context, in, false);
  context.parse_context.nerrors += file_context.nerrors;
  return true;
}
bool rychkov::Preprocessor::define(CommandContext& context)
{
  Macro macro = {get_name(context.in >> std::ws)};
  if (macro.name.empty())
  {
    log(context.parse_context, "macro must have name");
    return true;
  }
  if (std::isdigit(macro.name.front()))
  {
    log(context.parse_context, "macro name cannot start with digit");
    return true;
  }
  if (context.in.peek() == '(')
  {
    context.in.ignore(1);
    macro.func_style = true;
    while (true)
    {
      context.in >> std::ws;
      if (context.in.peek() == ')')
      {
        context.in.ignore(1);
        break;
      }
      std::string name = get_name(context.in);
      if (name.empty())
      {
        log(context.parse_context, "macro parameter must have name");
        return true;
      }
      if (std::isdigit(name.front()))
      {
        log(context.parse_context, "macro parameter name cannot start with digit (\"" + name + "\")");
        return true;
      }
      macro.parameters.push_back(std::move(name));
      if ((context.in >> std::ws).peek() == ',')
      {
        context.in.ignore(1);
      }
    }
  }
  std::getline(context.in >> std::ws, macro.body);
  for (; !macro.body.empty() && std::isspace(macro.body.back()); macro.body.pop_back())
  {}
  macros_.erase(macro);
  macros_.insert(std::move(macro));
  return true;
}
bool rychkov::Preprocessor::pragma(CommandContext& context)
{
  log(context.parse_context, "#pragma is unsupported");
  return true;
}
bool rychkov::Preprocessor::undef(CommandContext& context)
{
  std::string name;
  if (eol(context.in >> std::ws >> name) && !name.empty())
  {
    macros_.erase({name});
    return true;
  }
  log(context.parse_context, "wrong #undef format");
  return true;
}
bool rychkov::Preprocessor::ifdef(CommandContext& context)
{
  std::string name;
  if (!(context.in >> std::ws >> name) || !eol(context.in))
  {
    log(context.parse_context, "wrong macro name format");
    return true;
  }
  conditional_pairs_.push(macros_.find(name) == macros_.end() ? WAIT_ELSE : IF_BODY);
  return true;
}
bool rychkov::Preprocessor::ifndef(CommandContext& context)
{
  std::string name;
  if (!(context.in >> std::ws >> name) || !eol(context.in))
  {
    log(context.parse_context, "wrong macro name format");
    return true;
  }
  conditional_pairs_.push(macros_.find(name) != macros_.end() ? WAIT_ELSE : IF_BODY);
  return true;
}
bool rychkov::Preprocessor::else_cmd(CommandContext& context)
{
  if (!eol(context.in))
  {
    log(context.parse_context, "unexpected symbols after #else");
  }
  else if (conditional_pairs_.empty())
  {
    log(context.parse_context, "#else must be paired with #if directive");
  }
  else if ((conditional_pairs_.top() == ELSE_BODY) || (conditional_pairs_.top() == SKIP_ELSE))
  {
    log(context.parse_context, "#else duplicated");
  }
  else
  {
    conditional_pairs_.top() = (conditional_pairs_.top() == IF_BODY ? SKIP_ELSE : ELSE_BODY);
  }
  return true;
}
bool rychkov::Preprocessor::endif(CommandContext& context)
{
  if (!eol(context.in))
  {
    log(context.parse_context, "unexpected symbols after #endif");
  }
  else if (conditional_pairs_.empty())
  {
    log(context.parse_context, "#endif must be paired with #if directive");
  }
  else
  {
    conditional_pairs_.pop();
  }
  return true;
}
