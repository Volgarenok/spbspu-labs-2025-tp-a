#include "dictionary.hpp"
#include <ostream>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace
{
  struct KeyInDictChecker
  {
    KeyInDictChecker(const std::unordered_map< std::string, std::string > &dict, bool b):
      d(dict),
      exist(b)
    {}
    bool operator()(const std::pair< std::string, std::string > &p) const
    {
      if (exist)
      {
        return d.count(p.first) > 0;
      }
      return d.count(p.first) == 0;
    }
  private:
    const std::unordered_map< std::string, std::string > &d;
    bool exist;
  };
}

std::ostream &abramov::operator<<(std::ostream &out, const Dictionary &dict)
{
  for (const auto &pair : dict.dict_)
  {
    out << pair.first << " : " << pair.second << "\n";
  }
  return out;
}

abramov::Dictionary::Dictionary():
  dict_(std::unordered_map< std::string, std::string >{})
{}

abramov::Dictionary::Dictionary(const Dictionary &dict):
  dict_(std::unordered_map< std::string, std::string >(dict.dict_))
{}

abramov::Dictionary &abramov::Dictionary::operator=(const Dictionary &dict)
{
  if (this != std::addressof(dict))
  {
    Dictionary tmp(dict);
    swap(tmp);
  }
  return *this;
}

void abramov::Dictionary::addWord(const std::string &word, const std::string &trans)
{
  if (!dict_.insert({ word, trans }).second)
  {
    throw std::runtime_error("Fail to add word\n");
  }
}

void abramov::Dictionary::deleteWord(const std::string &word)
{
  if (!dict_.erase(word))
  {
    throw std::logic_error("There is no such word\n");
  }
}

void abramov::Dictionary::print(std::ostream &out) const noexcept
{
  out << *this;
}

abramov::Dictionary abramov::Dictionary::setWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  KeyInDictChecker f1(dict.dict_, false);
  std::copy_if(dict_.begin(), dict_.end(), std::inserter(res.dict_, res.dict_.end()), f1);
  KeyInDictChecker f2(dict_, false);
  std::copy_if(dict.dict_.begin(), dict.dict_.end(), std::inserter(res.dict_, res.dict_.end()), f2);
  return res;
}

abramov::Dictionary abramov::Dictionary::intersectWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  KeyInDictChecker f(dict.dict_, true);
  std::copy_if(dict_.begin(), dict_.end(), std::inserter(res.dict_, res.dict_.end()), f);
  return res;
}

abramov::Dictionary abramov::Dictionary::unionWithDict(const Dictionary &dict) const
{
  Dictionary res{};
  std::copy(dict_.begin(), dict_.end(), std::inserter(res.dict_, res.dict_.end()));
  std::copy(dict.dict_.begin(), dict.dict_.end(), std::inserter(res.dict_, res.dict_.end()));
  return res;
}

abramov::Dictionary abramov::Dictionary::diffDict(const Dictionary &dict)
{
  Dictionary res{};
  KeyInDictChecker f(dict.dict_, false);
  std::copy_if(dict_.begin(), dict_.end(), std::inserter(res.dict_, res.dict_.end()), f);
  return res;
}

abramov::Dictionary abramov::Dictionary::mergeDict(const Dictionary &dict)
{
  Dictionary res(*this);
  KeyInDictChecker f(res.dict_, false);
  std::copy_if(dict.dict_.begin(), dict.dict_.end(), std::inserter(res.dict_, res.dict_.end()), f);
  return res;
}

bool abramov::Dictionary::empty() const noexcept
{
  return dict_.empty();
}

void abramov::Dictionary::swap(Dictionary &dict) noexcept
{
  std::swap(dict_, dict.dict_);
}
