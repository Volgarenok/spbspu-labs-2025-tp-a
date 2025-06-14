#include "code_parsers.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>

rychkov::Parser::map_type< rychkov::Preprocessor::CommandContext, rychkov::Preprocessor >
    rychkov::Preprocessor::call_map = {
      {"include", &rychkov::Preprocessor::include},
      {"define", &rychkov::Preprocessor::define},
      {"pragma", &rychkov::Preprocessor::pragma},
      {"undef", &rychkov::Preprocessor::undef}
    };

rychkov::Preprocessor::Preprocessor():
  next_{nullptr}
{}
rychkov::Preprocessor::Preprocessor(Lexer* next, std::vector< std::string > search_dirs):
  next_{next},
  include_dirs_(std::move(search_dirs))
{}

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
void rychkov::Preprocessor::flush(CParseContext& context)
{
  for (char c: last_line)
  {
    flush(context, c);
  }
  flush(context, '\n');
  if (next_ != nullptr)
  {
    next_->flush(context);
  }
  last_line.clear();
}
void rychkov::Preprocessor::append(CParseContext& context, char c)
{
  if (c != '\n')
  {
    last_line += c;
    return;
  }
  std::string::size_type first = last_line.find_first_not_of(" \t\v\r");
  if ((first != std::string::npos) && (last_line[first] == '#'))
  {
    std::istringstream in(context.last_line.substr(first + 1));
    CommandContext cmd_context{in >> std::noskipws, context.err, context};
    rychkov::Parser::parse(cmd_context, *this, rychkov::Preprocessor::call_map);
    if (!in)
    {
      log(context, "unknown preprocessor command");
    }
    flush(context, '\n');
    last_line.clear();
    return;
  }
  flush(context);
}
void rychkov::Preprocessor::parse(CParseContext& context, std::istream& in)
{
  while (in)
  {
    std::string acc;
    std::getline(in, acc);
    while (!acc.empty() && (acc.back() == '\\'))
    {
      acc.erase(acc.end() - 1);
      std::string temp;
      std::getline(in, temp);
      acc += std::move(temp);
    }
    parse(context, std::move(acc));
  }
  flush(context);
}
void rychkov::Preprocessor::parse(CParseContext& context, std::string line)
{
  context.last_line = line;
  if (multiline_comment)
  {
    std::string::size_type multi_comment_end = line.find("*/");
    line.erase(0, multi_comment_end);
    if (multi_comment_end == std::string::npos)
    {
      return;
    }
  }

  std::string::size_type single_comment_start = line.find("//");
  std::string::size_type multi_comment_start = line.find("/*");
  std::string::size_type quotes_comment_start = line.find("\"");
  std::string::size_type last = 0;
  while ((single_comment_start != std::string::npos) || (multi_comment_start != std::string::npos)
      || (quotes_comment_start != std::string::npos))
  {
    std::string::size_type min = std::min({single_comment_start, multi_comment_start,
            quotes_comment_start});
    for (; last < min; last++)
    {
      append(context, line[last]);
      context.symbol++;
    }
    if (single_comment_start == min)
    {
      append(context, '\n');
      context.line++;
      context.symbol = 0;
      return;
    }
    if (multi_comment_start == min)
    {
      line.erase(multi_comment_start, 2);
      std::string::size_type multi_comment_end = line.find("*/");
      if (multi_comment_end == std::string::npos)
      {
        context.line++;
        context.symbol = 0;
        return;
      }
      context.symbol += 4 + multi_comment_end - multi_comment_start;
      line.erase(multi_comment_start, multi_comment_end - multi_comment_start + 2);
    }
    else
    {
      std::string::size_type quotes_comment_end = line.find("\"", quotes_comment_start + 1);
      if (quotes_comment_end == std::string::npos)
      {
        log(context, "string literal hasn't end symbol");
        return;
      }
      min = quotes_comment_end + 1;
      for (; last < min; last++)
      {
        append(context, line[last]);
        context.symbol++;
      }
    }
    single_comment_start = line.find("//", min);
    multi_comment_start = line.find("/*", min);
    quotes_comment_start = line.find("\"", min);
  }
  for (; last < line.length(); last++)
  {
    append(context, line[last]);
    context.symbol++;
  }
  append(context, '\n');
  context.line++;
  context.symbol = 0;
  return;
}
void rychkov::Preprocessor::CommandContext::parse_error()
{
  in.setstate(std::ios::failbit);
}
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

  last_line.clear();
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
  parse(file_context, in);
  context.parse_context.nerrors += file_context.nerrors;
  return true;
}
bool rychkov::Preprocessor::define(CommandContext& context)
{
  log(context.parse_context, "#define is unready");
  return true;
}
bool rychkov::Preprocessor::pragma(CommandContext& context)
{
  log(context.parse_context, "#pragma is unsupported");
  return true;
}
bool rychkov::Preprocessor::undef(CommandContext& context)
{
  log(context.parse_context, "#undef is unready");
  return true;
}
