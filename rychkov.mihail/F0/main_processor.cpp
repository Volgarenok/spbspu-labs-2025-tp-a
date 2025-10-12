#include "main_processor.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <utility>
#include <algorithm>

rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MainProcessor > rychkov::MainProcessor::call_map = {
      {"save", &rychkov::MainProcessor::save},
      {"load", &rychkov::MainProcessor::load},
      {"reload-all", &rychkov::MainProcessor::reload},
      {"parse", &rychkov::MainProcessor::parse},
      {"parse-after", &rychkov::MainProcessor::parse_after},
      {"tree", &rychkov::MainProcessor::tree},
      {"files", &rychkov::MainProcessor::files},
      {"external", &rychkov::MainProcessor::external},
      {"exposition", &rychkov::MainProcessor::exposition},
      {"defines", &rychkov::MainProcessor::defines},
      {"dependencies", &rychkov::MainProcessor::dependencies},
      {"uses", &rychkov::MainProcessor::uses},
      {"intersections", &rychkov::MainProcessor::intersections},
      {"diff", &rychkov::MainProcessor::diff},
      {"unopen-defines", &rychkov::MainProcessor::unopen_defines}
    };

rychkov::ParseCell::ParseCell(CParseContext context, Stage last_stage,
    std::vector< std::string > include_dirs):
  base_context{std::move(context)},
  preproc{std::unique_ptr< Lexer >{last_stage == PREPROCESSOR ? nullptr : new Lexer
        {std::unique_ptr< CParser >{last_stage != CPARSER ? nullptr : new CParser{}}}},
      std::move(include_dirs)}
{}
bool rychkov::ParseCell::parse(std::istream& in)
{
  std::stringstream cache_stream;
  cache_stream << in.rdbuf();
  cache = cache_stream.str();
  preproc.parse(base_context, cache_stream);
  return base_context.nerrors == 0;
}

bool rychkov::MainProcessor::parse(ParserContext& context)
{
  std::string filename;
  if (!(context.in >> filename) || !eol(context.in))
  {
    return false;
  }
  std::ifstream in(filename);
  if (!in)
  {
    context.err << "failed to open file\n";
    return true;
  }
  ParseCell cell = {{context.out, context.err, filename}, last_stage_, include_dirs_};
  context.out << "<--PARSE: \"" << filename << "\"-->\n";
  if (!cell.parse(in))
  {
    context.err << "failed to parse file \"" << filename << "\" - stopping\n";
    return true;
  }
  context.out << "<--DONE-->\n";
  parsed_.erase(filename);
  parsed_.emplace(filename, std::move(cell));
  return true;
}
bool rychkov::MainProcessor::reload(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  std::map< std::string, ParseCell > new_parsed;
  for (const std::pair< const std::string, ParseCell >& file: parsed_)
  {
    if (!file.second.real_file)
    {
      continue;
    }
    std::ifstream in(file.first);
    if (!in)
    {
      context.err << "failed to reopen source file: \"" << file.first << "\"\n";
    }
    std::pair< decltype(new_parsed)::iterator, bool > cell_p = new_parsed.emplace(file.first,
          ParseCell{{context.out, context.err, file.first}, last_stage_, include_dirs_});
    if (cell_p.second)
    {
      context.out << "<--PARSE: \"" << file.first << "\"-->\n";
      if (!cell_p.first->second.parse(in))
      {
        context.err << "failed to parse file \"" << file.first << "\" - stopping\n";
        return true;
      }
    }
  }
  context.out << "<--DONE-->\n";
  parsed_ = std::move(new_parsed);
  generated_files = 0;
  return true;
}
bool rychkov::MainProcessor::parse_after(ParserContext& context)
{
  std::string generated_name = "untitled_" + std::to_string(generated_files + 1);
  ParseCell cell = {{context.out, context.err, generated_name}, last_stage_, include_dirs_};
  cell.real_file = false;
  if (!eol(context.in))
  {
    std::string after;
    context.in >> after;
    const CParser& src_parser = *parsed_.at(after).preproc.next->next;
    if (!context.in || !eol(context.in))
    {
      return false;
    }
    CParser& dest_parser = *cell.preproc.next->next;
    dest_parser.aliases = src_parser.aliases;
    dest_parser.variables = src_parser.variables;
    dest_parser.structs = src_parser.structs;
    dest_parser.enums = src_parser.enums;
    dest_parser.unions = src_parser.unions;
  }
  while (context.in.good())
  {
    cell.base_context.symbol = 0;
    std::getline(context.in, cell.base_context.last_line);
    std::string::size_type eof_tag = cell.base_context.last_line.find("$EOF");
    if (eof_tag != std::string::npos)
    {
      cell.base_context.last_line.erase(eof_tag);
    }
    cell.cache += cell.base_context.last_line;
    cell.cache += '\n';
    for (char c: cell.base_context.last_line)
    {
      cell.preproc.append(cell.base_context, c);
      cell.base_context.symbol++;
    }
    if (eof_tag == std::string::npos)
    {
      cell.preproc.append(cell.base_context, '\n');
      cell.base_context.line++;
    }
    else
    {
      break;
    }
  }
  cell.preproc.flush(cell.base_context);
  if (cell.base_context.nerrors > 0)
  {
    context.err << "failed to parse - delete temporary\n";
    return true;
  }
  context.out << "<--GENERETED: \"" << generated_name << "\"-->\n";
  generated_files++;
  parsed_.emplace(generated_name, std::move(cell));
  return true;
}

bool rychkov::MainProcessor::save(ParserContext& context)
{
  std::string result;
  if (!eol(context.in))
  {
    context.in >> result;
    if (!context.in || !eol(context.in))
    {
      return false;
    }
  }
  else
  {
    result = save_file_;
  }
  if (save(context.err, result))
  {
    context.out << "saved to \"" << result << "\"\n";
  }
  return true;
}
bool rychkov::MainProcessor::load(ParserContext& context)
{
  std::string source;
  if (!eol(context.in))
  {
    context.in >> source;
    if (!context.in || !eol(context.in))
    {
      return false;
    }
  }
  else
  {
    source = save_file_;
  }
  context.out << "load from \"" << source << "\"\n";
  if (load(context.out, context.err, source))
  {
    context.out << "<--DONE-->\n";
  }
  return true;
}
bool rychkov::MainProcessor::unopen_defines(ParserContext& context)
{
  std::string filename;
  if ((last_stage_ != CPARSER) || !(context.in >> filename))
  {
    return false;
  }
  const ParseCell& cell = parsed_.at(filename);
  if (!eol(context.in))
  {
    return false;
  }
  std::istringstream in(cell.cache);
  std::stringstream preprocessed;
  Preprocessor preproc{nullptr, include_dirs_};
  CParseContext parse_context{preprocessed, context.err, cell.base_context.file};
  preproc.parse(parse_context, in);
  std::string line;
  size_t line_number = 1;
  size_t nfound = 0;
  while (preprocessed.good())
  {
    struct is_name_char
    {
      bool operator()(unsigned char c)
      {
        return (c == '_') || std::isalnum(c);
      }
    };
    std::getline(preprocessed, line);
    std::string::iterator from = std::find_if(line.begin(), line.end(), is_name_char{});
    while (from != line.end())
    {
      std::string::iterator to = std::find_if_not(from, line.end(), is_name_char{});
      std::string name{from, to};
      if ((preproc.macros.find(name) != preproc.macros.end())
            || (preproc.legacy_macros.find(name) != preproc.legacy_macros.end()))
      {
        std::cout << "* found macro name \"" << name << "\" on line " << line_number << ":\n";
        std::cout << line << '\n';
        nfound++;
      }
      from = std::find_if(to, line.end(), is_name_char{});
    }
    line_number++;
  }
  std::cout << "* summary: found " << nfound << " unopen macro names\n";
  return true;
}
