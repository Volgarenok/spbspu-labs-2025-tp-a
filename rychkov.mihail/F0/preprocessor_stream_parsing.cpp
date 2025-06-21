#include "preprocessor.hpp"

#include <iostream>
#include <utility>
#include <algorithm>

void rychkov::Preprocessor::parse(CParseContext& context, std::istream& in, bool need_flush)
{
  bool not_first = false;
  while (in.good())
  {
    if (not_first && !multiline_comment_)
    {
      append(context, '\n');
    }
    not_first = true;
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
  if (need_flush)
  {
    flush(context);
  }
}
void rychkov::Preprocessor::parse(CParseContext& context, std::string line)
{
  context.last_line = line;
  if (multiline_comment_)
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
      bool screened = false;
      while (true)
      {
        std::string::size_type quotes_comment_end = line.find("\"", min + 1);
        if (quotes_comment_end == std::string::npos)
        {
          context.symbol += line.length() - min + 1;
          log(context, "string literal hasn't end symbol");
          return;
        }
        for (; last < quotes_comment_end; last++)
        {
          screened = false;
          append(context, line[last]);
          context.symbol++;
          if (line[last] == '\\')
          {
            screened = true;
          }
        }
        append(context, '"');
        last++;
        min = quotes_comment_end + 1;
        if (!screened)
        {
          break;
        }
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
  context.line++;
  context.symbol = 0;
  return;
}
