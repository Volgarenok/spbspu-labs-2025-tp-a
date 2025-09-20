#include "preprocessor.hpp"

#include <iostream>
#include <fstream>
#include <utility>
#include <cctype>
#include <parser.hpp>

void rychkov::Preprocessor::include(std::istream& in, CParseContext& context)
{
  char quote = '\0';
  if (!(in >> std::ws >> quote))
  {
    log(context, "no filename");
    return;
  }
  std::string filename;
  if ((quote != '"') && (quote != '<'))
  {
    log(context, "wrong quotes");
    return;
  }
  std::getline(in, filename, (quote == '"' ? '"' : '>'));
  if (in.eof())
  {
    log(context, "missing end quote");
    return;
  }
  if (!(in >> std::ws).eof())
  {
    log(context, "text after filename");
    return;
  }

  buf_.clear();
  std::ifstream file;
  if (quote == '"')
  {
    file.open(filename);
  }
  else
  {
    for (const std::string& base_path: include_paths)
    {
      file.open(base_path + '/' + filename);
      if (file.is_open())
      {
        filename = base_path + '/' + filename;
        break;
      }
    }
  }
  if (!file)
  {
    log(context, "failed to open file");
    return;
  }
  CParseContext file_context = {context.out, context.err, filename, &context};
  parse(file_context, file, false);
  context.nerrors += file_context.nerrors;
}
void rychkov::Preprocessor::define(std::istream& in, CParseContext& context)
{
  Macro macro = {get_name(in >> std::ws)};
  if (macro.name.empty())
  {
    log(context, "macro must have name");
    return;
  }
  if (std::isdigit(macro.name.front()))
  {
    log(context, "macro name cannot start with digit");
    return;
  }
  if (in.peek() == '(')
  {
    in.ignore(1);
    macro.func_style = true;
    while (true)
    {
      if ((in >> std::ws).peek() == ')')
      {
        in.ignore(1);
        break;
      }
      std::string name = get_name(in);
      if (name.empty())
      {
        log(context, "macro parameter must have name");
        return;
      }
      if (std::isdigit(name.front()))
      {
        log(context, "macro parameter name cannot start with digit (\"" + name + "\")");
        return;
      }
      macro.parameters.push_back(std::move(name));
      if ((in >> std::ws).peek() == ',')
      {
        in.ignore(1);
      }
    }
  }
  std::getline(in >> std::ws, macro.body);
  remove_whitespaces(macro.body);
  macros.erase(macro);
  macros.insert(std::move(macro));
}
void rychkov::Preprocessor::pragma(std::istream&, CParseContext& context)
{
  log(context, "#pragma is unsupported");
}
void rychkov::Preprocessor::undef(std::istream& in, CParseContext& context)
{
  std::string name;
  if (eol(in >> std::ws >> name) && !name.empty())
  {
    decltype(macros)::iterator temp = macros.find(name);
    if (temp != macros.end())
    {
      legacy_macros.insert(*temp);
      macros.erase(temp);
    }
    return;
  }
  log(context, "wrong #undef format");
}
void rychkov::Preprocessor::ifdef(std::istream& in, CParseContext& context)
{
  std::string name;
  if (!(in >> std::ws >> name) || !eol(in))
  {
    log(context, "wrong macro name format");
    return;
  }
  conditional_pairs_.push(macros.find(name) == macros.end() ? WAIT_ELSE : IF_BODY);
}
void rychkov::Preprocessor::ifndef(std::istream& in, CParseContext& context)
{
  std::string name;
  if (!(in >> std::ws >> name) || !eol(in))
  {
    log(context, "wrong macro name format");
    return;
  }
  conditional_pairs_.push(macros.find(name) != macros.end() ? WAIT_ELSE : IF_BODY);
}
void rychkov::Preprocessor::else_cmd(std::istream& in, CParseContext& context)
{
  if (!eol(in))
  {
    log(context, "unexpected symbols after #else");
  }
  else if (conditional_pairs_.empty())
  {
    log(context, "#else must be paired with #if directive");
  }
  else if ((conditional_pairs_.top() == ELSE_BODY) || (conditional_pairs_.top() == SKIP_ELSE))
  {
    log(context, "#else duplicated");
  }
  else
  {
    conditional_pairs_.top() = (conditional_pairs_.top() == IF_BODY ? SKIP_ELSE : ELSE_BODY);
  }
}
void rychkov::Preprocessor::endif(std::istream& in, CParseContext& context)
{
  if (!eol(in))
  {
    log(context, "unexpected symbols after #endif");
  }
  else if (conditional_pairs_.empty())
  {
    log(context, "#endif must be paired with #if directive");
  }
  else
  {
    conditional_pairs_.pop();
  }
}
