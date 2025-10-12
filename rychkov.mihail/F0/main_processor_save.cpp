#include "main_processor.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <boost/json.hpp>
#include "print_content.hpp"

namespace rychkov
{
  struct Serializer
  {
    std::string operator()(const typing::Type& type);
    boost::json::value operator()(const Macro& macro);
    boost::json::value operator()(const entities::Variable& var);
    boost::json::value operator()(const entities::Function& func);
    boost::json::value operator()(const entities::Body& body);
    boost::json::value operator()(const entities::Statement& statement);
    boost::json::value operator()(const entities::Struct& structure);
    boost::json::value operator()(const entities::Enum& structure);
    boost::json::value operator()(const entities::Union& structure);
    boost::json::value operator()(const entities::Alias& alias);
    boost::json::value operator()(const entities::Declaration& decl);

    boost::json::value operator()(const entities::Literal& literal);
    boost::json::value operator()(const entities::CastOperation& cast);

    boost::json::value operator()(const DynMemWrapper< entities::Expression >& root);
    boost::json::value operator()(const entities::Expression& root);
    boost::json::value operator()(const entities::Expression::operand& operand);
  };
}

bool rychkov::MainProcessor::save(std::ostream& err, std::string filename) const
{
  std::ofstream out(filename);
  if (!out)
  {
    err << "failed to open save file on write - \"" << filename << "\"\n";
    return false;
  }
  boost::json::object doc;
  for (const std::pair< const std::string, ParseCell >& file: parsed_)
  {
    const Preprocessor& preproc = file.second.preproc;
    const CParser& src = *preproc.next->next;
    Serializer serial;
    boost::json::array macros(preproc.macros.size());
    boost::json::array legacy_macros(preproc.legacy_macros.size());
    boost::json::array program;
    std::transform(preproc.macros.begin(), preproc.macros.end(), macros.begin(), serial);
    std::transform(preproc.legacy_macros.begin(), preproc.legacy_macros.end(), legacy_macros.begin(), serial);
    std::transform(src.begin(), src.end(), std::back_inserter(program), serial);
    doc[file.first] = boost::json::object{{"macros", std::move(macros)},
          {"old_macro", std::move(legacy_macros)}, {"pgm", std::move(program)},
          {"real", file.second.real_file}, {"cache", file.second.cache}};
  }
  out << doc << '\n';
  return out.good();
}

std::string rychkov::Serializer::operator()(const typing::Type& type)
{
  std::ostringstream temp;
  temp << type;
  return temp.str();
}
boost::json::value rychkov::Serializer::operator()(const Macro& macro)
{
  return boost::json::object{{"name", macro.name}, {"fstyle", macro.func_style}, {"body", macro.body},
        {"params", boost::json::array(macro.parameters.begin(), macro.parameters.end())}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Variable& var)
{
  std::ostringstream temp;
  temp << var;
  return boost::json::object{{"obj", "var"}, {"data", temp.str()}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Function& func)
{
  boost::json::array parameters(func.parameters.size());
  std::copy(func.parameters.begin(), func.parameters.end(), parameters.begin());
  return boost::json::object{{"obj", "func"}, {"parameters", std::move(parameters)},
        {"sign", operator()(entities::Variable{func.type, func.name})}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Body& body)
{
  boost::json::array result(body.data.size());
  std::transform(body.data.begin(), body.data.end(), result.begin(), *this);
  return result;
}
boost::json::value rychkov::Serializer::operator()(const entities::Statement& statement)
{
  boost::json::array conditions(statement.conditions.size());
  std::transform(statement.conditions.begin(), statement.conditions.end(), conditions.begin(), *this);
  return boost::json::object{{"obj", "stmtnt"}, {"type", statement.type}, {"conditions", std::move(conditions)}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Struct& structure)
{
  boost::json::array fields(structure.fields.size());
  std::transform(structure.fields.begin(), structure.fields.end(), fields.begin(), *this);
  return boost::json::object{{"obj", "struct"}, {"name", structure.name}, {"fields", std::move(fields)}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Enum& structure)
{
  return boost::json::object{{"obj", "enum"}, {"name", structure.name},
        {"fields", boost::json::object(structure.fields.begin(), structure.fields.end())}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Union& structure)
{
  boost::json::array fields(structure.fields.size());
  std::transform(structure.fields.begin(), structure.fields.end(), fields.begin(), *this);
  return boost::json::object{{"obj", "union"}, {"name", structure.name}, {"fields", std::move(fields)}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Alias& alias)
{
  return boost::json::object{{"obj", "alias"}, {"sign", operator()(entities::Variable{alias.type, alias.name})}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Declaration& decl)
{
  return boost::json::object{{"obj", "decl"}, {"data", boost::variant2::visit(*this, decl.data)},
        {"value", operator()(decl.value)}, {"scope", decl.scope}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Literal& lit)
{
  return boost::json::object{{"obj", "lit"}, {"body", lit.literal}, {"suf", lit.suffix},
        {"type", lit.type}, {"res_type", operator()(lit.result_type)}};
}
boost::json::value rychkov::Serializer::operator()(const entities::CastOperation& cast)
{
  return boost::json::object{{"obj", "cast"}, {"to", operator()(cast.to)}, {"explicit", cast.is_explicit},
        {"value", operator()(cast.expr)}};
}
boost::json::value rychkov::Serializer::operator()(const entities::Expression& expr)
{
  if (expr.empty())
  {
    return boost::json::object{};
  }
  if (entities::is_bridge(expr))
  {
    return operator()(expr.operands[0]);
  }
  boost::json::array operands(expr.operands.size());
  std::transform(expr.operands.begin(), expr.operands.end(), operands.begin(), *this);
  return boost::json::object{{"obj", "expr"}, {"token", expr.operation->token},
        {"rallign", expr.operation->right_align}, {"size", expr.operation->type},
        {"operands", std::move(operands)}, {"res_t", operator()(expr.result_type)}};
}
boost::json::value rychkov::Serializer::operator()(const DynMemWrapper< entities::Expression >& ptr)
{
  return ptr != nullptr ? operator()(*ptr) : boost::json::value{};
}
boost::json::value rychkov::Serializer::operator()(const entities::Expression::operand& root)
{
  return boost::variant2::visit(*this, root);
}
