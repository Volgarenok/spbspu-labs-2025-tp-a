#include "type_parser.hpp"

#include <utility>

void rychkov::TypeParser::remove_combination(typing::Type& type)
{
  if (type.category == typing::COMBINATION)
  {
    DynMemWrapper< typing::Type > temp = std::move(type.base);
    temp->is_const = type.is_const || temp->is_const;
    temp->is_volatile = type.is_volatile || temp->is_volatile;
    type = std::move(*temp);
  }
}
rychkov::typing::Type* rychkov::TypeParser::move_up()
{
  remove_combination(*stack_.top().data);
  typing::Type* old = new typing::Type{std::move(*stack_.top().data)};
  *stack_.top().data = {{}, typing::COMBINATION, old};
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
bool rychkov::TypeParser::is_type() const noexcept
{
  return !empty() && var_name_.empty();
}
bool rychkov::TypeParser::is_function() const noexcept
{
  return !empty() && (combined_.category == typing::FUNCTION);
}

void rychkov::TypeParser::prepare()
{
  remove_combination(combined_);
}
rychkov::typing::Type rychkov::TypeParser::type() const
{
  remove_combination(*stack_.top().data);
  return combined_;
}
rychkov::entities::Variable rychkov::TypeParser::variable() const
{
  remove_combination(*stack_.top().data);
  return {combined_, var_name_};
}
rychkov::entities::Function rychkov::TypeParser::function() const
{
  remove_combination(*stack_.top().data);
  return {combined_, var_name_, parameters_};
}
std::string rychkov::TypeParser::name() const
{
  return var_name_;
}
void rychkov::TypeParser::clear()
{
  combined_ = {};
  stack_ = {};
  var_name_.clear();
  parameters_.clear();
}

void rychkov::TypeParser::append(CParseContext& context, unsigned long long numeric_literal)
{
  if (stack_.empty() || !stack_.top().bracket_opened)
  {
    log(context, "number cannot be parsed here");
    return;
  }
  stack_.top().data->array_has_length = true;
  stack_.top().data->array_length = numeric_literal;
}
void rychkov::TypeParser::append(CParseContext& context, typing::Type base_type)
{
  if (stack_.empty())
  {
    combined_ = std::move(base_type);
    stack_.push({&combined_});
    return;
  }
  if (!stack_.top().data->name.empty() || stack_.top().data->base != nullptr)
  {
    log(context, "types cannot be merged here");
    return;
  }
  if ((base_type.category == typing::ARRAY) || (base_type.category == typing::FUNCTION))
  {
    if (stack_.top().data->is_const || stack_.top().data->is_volatile)
    {
      log(context, "const cannot be applied to array or function");
      return;
    }
  }
  if ((base_type.category != typing::BASIC) && (stack_.top().data->is_signed || stack_.top().data->is_unsigned))
  {
    log(context, "signed or unsigned cannot be applied not to integer type");
    return;
  }
  stack_.top().data->base = std::move(base_type);
}
