#include "log.hpp"

#include <iostream>

void rychkov::log(CParseContext& context, std::string message)
{
  context.err << "In file \"" << context.file << "\" (" << context.line + 1 << ':' << context.symbol + 1 << ")\n";
  context.err << "\tError: " << message << "\n  ";
  context.err << context.last_line << "\n  " << std::string(context.symbol, '-') << "^\n";
  for (const CParseContext* file = context.base; file != nullptr; file = file->base)
  {
    context.err << "Included from \"" << file->file << "\" (" << file->line + 1 << ':' << file->symbol + 1 << ")\n  ";
    context.err << file->last_line << "\n  " << std::string(file->symbol, '-') << "^\n";
  }
  context.nerrors++;
}
