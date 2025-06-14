#include "processors.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <utility>
#include <stdexcept>

using namespace std::literals::string_literals;

rychkov::Parser::map_type< rychkov::ParserContext, rychkov::MainProcessor > rychkov::MainProcessor::call_map = {
      {"help", &rychkov::MainProcessor::help}
    };

rychkov::MainProcessor::ParseCell::ParseCell(CParseContext context, Stage last_stage,
    std::vector< std::string > include_dirs):
  base_context{std::move(context)},
  preproc{(last_stage == PREPROCESSOR ? nullptr : &lex), std::move(include_dirs)},
  lex{(last_stage == PREPROCESSOR) || (last_stage == LEXER) ? nullptr : &parser},
  parser{}
{}
bool rychkov::MainProcessor::ParseCell::parse(std::istream& in)
{
  preproc.parse(base_context, in);
  return base_context.nerrors == 0;
}
bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  bool sources = false;
  std::vector< std::string > files;
  std::ofstream out;
  for (size_t i = 1; i < argc; i++)
  {
    if (std::strcmp(argv[i], "--help") == 0)
    {
      help(context);
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
  if (out.is_open())
  {
    for (const std::string& filename: files)
    {
      std::ifstream in(filename);
      if (!in)
      {
        throw std::runtime_error("failed to open source file: \""s + filename + '"');
      }
      CParseContext parse_context{out, context.err, filename};
      ParseCell cell{parse_context, last_stage_, include_dirs_};
      if (!cell.parse(in))
      {
        if (last_stage_ == CPARSER)
        {
          cell.parser.print(out);
        }
        throw std::runtime_error("failed to parse file \"" + filename + "\" - stopping");
      }
      if (last_stage_ == CPARSER)
      {
        cell.parser.print(out);
      }
    }
    return false;
  }
  return true;
}
bool rychkov::MainProcessor::help(ParserContext& context)
{
  context.out << "<[help info]>\n";
  return true;
}
