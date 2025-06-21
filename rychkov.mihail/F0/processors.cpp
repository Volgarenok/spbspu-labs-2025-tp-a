#include "processors.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <utility>
#include <stdexcept>

using namespace std::literals::string_literals;

rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MainProcessor > rychkov::MainProcessor::call_map = {
      {"tree", &rychkov::MainProcessor::tree},
      {"files", &rychkov::MainProcessor::parsed},
      {"reload-all", &rychkov::MainProcessor::reload}
    };

rychkov::ParseCell::ParseCell(CParseContext context, Stage last_stage,
    std::vector< std::string > include_dirs):
  base_context{std::move(context)},
  preproc{(last_stage == PREPROCESSOR ? nullptr : &lex), std::move(include_dirs)},
  lex{(last_stage == PREPROCESSOR) || (last_stage == LEXER) ? nullptr : &parser},
  parser{}
{}
bool rychkov::ParseCell::parse(std::istream& in)
{
  preproc.parse(base_context, in);
  return base_context.nerrors == 0;
}
bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  bool sources = false;
  std::vector< std::string > files;
  std::ofstream out;
  for (int i = 1; i < argc; i++)
  {
    if (std::strcmp(argv[i], "--help") == 0)
    {
      help(context.out);
      return false;
    }
    if (std::strcmp(argv[i], "-c") == 0)
    {
      sources = true;
    }
    else if (std::strcmp(argv[i], "-E") == 0)
    {
      if (last_stage_ != CPARSER)
      {
        throw std::invalid_argument("stop flag cannot be specified more then once");
      }
      last_stage_ = PREPROCESSOR;
      sources = false;
    }
    else if (std::strcmp(argv[i], "-L") == 0)
    {
      if (last_stage_ != CPARSER)
      {
        throw std::invalid_argument("stop flag cannot be specified more then once");
      }
      last_stage_ = LEXER;
      sources = false;
    }
    else if (std::strcmp(argv[i], "-o") == 0)
    {
      sources = false;
      if (out.is_open())
      {
        throw std::invalid_argument("duplicating -o flag");
      }
      if ((++i >= argc) || (argv[i][0] == '\0'))
      {
        throw std::invalid_argument("missing output filename");
      }
      out.open(argv[i]);
      if (!out)
      {
        throw std::runtime_error("failed to open output file: \""s + argv[i] + '"');
      }
    }
    else if (std::strcmp(argv[i], "-I") == 0)
    {
      sources = false;
      if (++i >= argc)
      {
        throw std::invalid_argument("missing include directory");
      }
      include_dirs_.push_back(argv[i]);
    }
    else
    {
      if (!sources)
      {
        throw std::invalid_argument("unknown command: \""s + argv[i] + '"');
      }
      files.push_back(argv[i]);
    }
  }

  for (const std::string& filename: files)
  {
    CParseContext parse_context{(out.is_open() ? out : context.out), context.err, filename};
    context.out << "<--PARSE: \"" << filename << "\"-->\n";
    if (!parse(parse_context, false))
    {
      throw std::runtime_error("failed to parse file \"" + filename + "\" - stopping");
    }
  }
  context.out << "<--DONE-->\n";
  return !out.is_open();
}

void rychkov::MainProcessor::help(std::ostream& out)
{
  out << "<[help info]>\n";
}
bool rychkov::MainProcessor::parse(CParseContext file_context, bool overwrite)
{
  std::pair< decltype(parsed_)::iterator, bool > cell = parsed_.try_emplace(file_context.file,
        file_context, last_stage_, include_dirs_);
  if (!cell.second && !overwrite)
  {
    return true;
  }
  std::ifstream in(cell.first->first);
  if (!in)
  {
    throw std::invalid_argument("failed to open source file: \"" + file_context.file + '"');
  }
  return cell.first->second.parse(in);
}

bool rychkov::MainProcessor::parsed(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  for (const decltype(parsed_)::value_type& i: parsed_)
  {
    context.out << i.first << '\n';
  }
  return true;
}
bool rychkov::MainProcessor::tree(ParserContext& context)
{
  std::string filename;
  if ((last_stage_ != CPARSER) || !(context.in >> filename) || !eol(context.in))
  {
    return false;
  }
  parsed_.at(filename).parser.print(context.out);
  return true;
}
bool rychkov::MainProcessor::reload(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  std::map< std::string, ParseCell > new_parsed;
  for (const std::pair< std::string, ParseCell >& file: parsed_)
  {
    CParseContext parse_context{context.out, context.err, file.first};
    std::ifstream in(file.first);
    if (!in)
    {
      context.err << "failed to reopen source file: \"" << file.first << "\"\n";
    }
    std::pair< decltype(new_parsed)::iterator, bool > cell_p = new_parsed.try_emplace(file.first,
          std::move(parse_context), last_stage_, include_dirs_);
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
  return true;
}
