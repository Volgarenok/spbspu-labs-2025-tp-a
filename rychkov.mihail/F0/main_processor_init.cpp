#include "main_processor.hpp"

#include <fstream>
#include <cstring>
#include <utility>
#include <stdexcept>
#include <algorithm>

using namespace std::literals::string_literals;

bool rychkov::MainProcessor::init(ParserContext& context, int argc, char** argv)
{
  bool sources = false;
  bool out = false;
  std::vector< std::string > files;
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
    else if (std::strcmp(argv[i], "--load") == 0)
    {
      sources = false;
      if (++i >= argc)
      {
        throw std::invalid_argument("missing save-file name");
      }
      if (!load(context.out, context.err, argv[i]))
      {
        throw std::runtime_error("failed to load save-file \""s + argv[i] + '"');
      }
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
      if (out)
      {
        throw std::invalid_argument("out flag duplicates");
      }
      out = true;
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

  std::sort(files.begin(), files.end());
  files.erase(std::unique(files.begin(), files.end()), files.end());

  std::string ext = (last_stage_ == PREPROCESSOR ? ".i" : (last_stage_ == LEXER ? ".lex" : ".json"));
  for (const std::string& filename: files)
  {
    std::ostream* output = &context.out;
    std::ofstream ostream;
    std::string output_filename;
    if (out)
    {
      std::string::size_type ext_p = filename.rfind(".c");
      if (ext_p == std::string::npos)
      {
        output_filename = filename + ext;
      }
      else
      {
        output_filename = filename;
        output_filename.replace(ext_p, 2, ext);
      }
      ostream.open(output_filename);
      if (!ostream)
      {
        throw std::runtime_error("failed to open output file: \"" + output_filename + '"');
      }
      output = &ostream;
    }
    CParseContext parse_context{*output, context.err, filename};
    context.out << "<--PARSE: \"" << filename << "\"-->\n";
    if (!parse(parse_context, true))
    {
      throw std::runtime_error("failed to parse file \"" + filename + "\" - stopping");
    }
    if (out && (last_stage_ == CPARSER))
    {
      save(context.err, output_filename);
      parsed_.clear();
    }
  }
  context.out << "<--DONE-->\n";
  return !out;
}

bool rychkov::MainProcessor::parse(CParseContext file_context, bool overwrite)
{
  if (overwrite)
  {
    parsed_.erase(file_context.file);
  }
  std::pair< decltype(parsed_)::iterator, bool > cell = parsed_.emplace(file_context.file,
    ParseCell{file_context, last_stage_, include_dirs_});
  if (!cell.second)
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
