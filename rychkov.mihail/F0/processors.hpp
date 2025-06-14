#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include <string>
#include <vector>
#include <parser.hpp>
#include "code_parsers.hpp"

namespace rychkov
{
  class MainProcessor
  {
  public:
    static Parser::map_type< ParserContext, MainProcessor > call_map;

    bool init(ParserContext& context, int argc, char** argv);
    bool help(ParserContext& context);
  private:
    enum Stage
    {
      PREPROCESSOR,
      LEXER,
      CPARSER
    };
    struct ParseCell
    {
      ParseCell(CParseContext context, Stage last_stage, std::vector< std::string > include_dirs);
      bool parse(std::istream& in);
      CParseContext base_context;
      Preprocessor preproc;
      Lexer lex;
      CParser parser;
    };

    Stage last_stage_ = CPARSER;
    std::vector< std::string > include_dirs_;
  };
}

#endif
