#ifndef PROCESSORS_HPP
#define PROCESSORS_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include <map>

#include <parser.hpp>

#include "log.hpp"
#include "preprocessor.hpp"
#include "lexer.hpp"
#include "cparser.hpp"

namespace rychkov
{
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

  class MainProcessor
  {
  public:
    static Parser::map_type< ParserContext, MainProcessor > call_map;

    void help(std::ostream& out);
    bool parse(CParseContext file_context, bool overwrite);

    bool init(ParserContext& context, int argc, char** argv);
    bool tree(ParserContext& context);
    bool parsed(ParserContext& context);
    bool reload(ParserContext& context);

  private:
    Stage last_stage_ = CPARSER;
    std::vector< std::string > include_dirs_;
    std::map< std::string, ParseCell > parsed_;
  };
}

#endif
