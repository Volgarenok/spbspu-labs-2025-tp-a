#include "main_processor.hpp"

#include <fstream>
#include <algorithm>
#include "print_content.hpp"

void rychkov::MainProcessor::help(std::ostream& out)
{
  std::ifstream file(help_file);
  if (!file)
  {
    out << "failed to open help file - \"" << help_file << "\"\n";
    return;
  }
  out << file.rdbuf();
}
bool rychkov::MainProcessor::files(ParserContext& context)
{
  if (!eol(context.in))
  {
    return false;
  }
  ContentPrinter printer{context.out};
  for (const decltype(parsed_)::value_type& i: parsed_)
  {
    printer(i.first);
  }
  return true;
}
bool rychkov::MainProcessor::tree(ParserContext& context)
{
  std::string filename;
  if ((last_stage_ != CPARSER) || !(context.in >> filename))
  {
    return false;
  }
  const CParser& parser = *parsed_.at(filename).preproc.next->next;
  if (!eol(context.in))
  {
    return false;
  }
  std::for_each(parser.begin(), parser.end(), ContentPrinter{context.out});
  return true;
}
bool rychkov::MainProcessor::external(ParserContext& context)
{
  if ((last_stage_ != CPARSER) || !eol(context.in))
  {
    return false;
  }
  ContentPrinter printer{context.out};
  bool no_external = true;
  for (const std::pair< const std::string, ParseCell >& cell: parsed_)
  {
    for (const entities::Expression& expr: *cell.second.preproc.next->next)
    {
      if (entities::is_decl(expr))
      {
        const entities::Declaration& decl = boost::variant2::get< entities::Declaration >(expr.operands[0]);
        if (decl.scope == entities::EXTERN)
        {
          printer(decl);
          no_external = false;
        }
      }
    }
  }
  if (no_external)
  {
    context.out << "no external symbols\n";
  }
  return true;
}
bool rychkov::MainProcessor::exposition(ParserContext& context)
{
  std::string filename, structure;
  if ((last_stage_ != CPARSER) || !(context.in >> filename >> structure))
  {
    return false;
  }
  const CParser& parser = *parsed_.at(filename).preproc.next->next;
  if (!eol(context.in))
  {
    return false;
  }
  decltype(parser.structs)::iterator struct_p = parser.structs.find(structure);
  decltype(parser.unions)::iterator union_p = parser.unions.find(structure);
  decltype(parser.enums)::iterator enum_p = parser.enums.find(structure);
  ContentPrinter printer(context.out);
  if (struct_p != parser.structs.end())
  {
    printer(struct_p->first);
  }
  else if (union_p != parser.unions.end())
  {
    printer(union_p->first);
  }
  else if (enum_p != parser.enums.end())
  {
    printer(enum_p->first);
  }
  else
  {
    context.out << "no struct/enum/union with name \"" << structure << "\" in file \"" << filename << "\"\n";
  }
  return true;
}
bool rychkov::MainProcessor::defines(ParserContext& context)
{
  std::string filename;
  if ((last_stage_ != CPARSER) || !(context.in >> filename))
  {
    return false;
  }
  const Preprocessor& preproc = parsed_.at(filename).preproc;
  if (!eol(context.in))
  {
    return false;
  }
  ContentPrinter printer(context.out, 1);
  context.out << "<actual>:\n";
  std::for_each(preproc.macros.begin(), preproc.macros.end(), printer);
  context.out << "<legacy>:\n";
  std::for_each(preproc.legacy_macros.begin(), preproc.legacy_macros.end(), printer);
  return true;
}
