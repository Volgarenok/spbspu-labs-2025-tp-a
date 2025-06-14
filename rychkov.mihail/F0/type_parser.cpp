#include "code_parsers.hpp"

rychkov::typing::Type* rychkov::TypeParser::move_up()
{
  typing::Type* old = new typing::Type{std::move(*stack_.top().data)};
  *stack_.top().data = {{}, typing::Type::Combination, old};
  return old;
}
bool rychkov::TypeParser::can_be_named_param(typing::Type* type_p)
{
  return is_function() && !combined_.function_parameters.empty()
      && (&combined_.function_parameters.back() == type_p);
}
bool rychkov::TypeParser::empty() const noexcept
{
  return stack_.empty();
}
bool rychkov::TypeParser::ready() const noexcept
{
  return (stack_.size() == 1) && !stack_.top().bracket_opened && !stack_.top().parenthesis_opened;
}
bool rychkov::TypeParser::is_function() const noexcept
{
  return !empty() && (combined_.category == typing::Type::Function);
}

rychkov::typing::Type rychkov::TypeParser::type() const
{
  return combined_;
}
rychkov::entities::Variable rychkov::TypeParser::variable() const
{
  return {combined_, var_name_};
}
rychkov::entities::Function rychkov::TypeParser::function() const
{
  return {combined_, var_name_, parameters_};
}
void rychkov::TypeParser::clear()
{
  combined_ = {};
  stack_ = {};
  var_name_.clear();
  parameters_.clear();
}

bool rychkov::TypeParser::append(CParseContext& context, size_t numeric_literal)
{
  if (stack_.empty() || !stack_.top().bracket_opened)
  {
    log(context, "number cannot be parsed here");
    return false;
  }
  stack_.top().data->array_has_length = true;
  stack_.top().data->array_length = numeric_literal;
  return true;
}
bool rychkov::TypeParser::append(CParseContext& context, typing::Type base_type)
{
  if (stack_.empty())
  {
    combined_ = std::move(base_type);
    stack_.push({&combined_});
    return true;
  }
  if (stack_.top().data->base != nullptr)
  {
    log(context, "types cannot be merged here");
    return false;
  }
  if ((base_type.category == typing::Type::Array) || (base_type.category == typing::Type::Function))
  {
    if (stack_.top().data->is_const || stack_.top().data->is_volatile)
    {
      log(context, "const cannot be applied to array or function");
      return false;
    }
  }
  if ((base_type.category != typing::Type::Int) && (stack_.top().data->is_signed || stack_.top().data->is_unsigned))
  {
    log(context, "signed or unsigned cannot be applied not to integer type");
    return false;
  }
  stack_.top().data->base = std::move(base_type);
  return true;
}
