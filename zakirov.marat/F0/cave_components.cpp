#include "cave_components.hpp"
#include <algorithm>
#include <iostream>
#include <queue>
#include <stream_guardian.hpp>

namespace
{
  struct MinorSymbol
  {
    char symbol;
  };

  std::istream & operator>>(std::istream & in, MinorSymbol && sym)
  {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
      return in;
    }

    char symbol;
    if (!(in >> symbol) || symbol != sym.symbol)
    {
      in.setstate(std::ios::failbit);
    }

    return in;
  }
}

char zakirov::CaveComponent::getCell() const noexcept
{
  return cell_;
}

char zakirov::CaveComponent::setCell(char cell) noexcept
{
  cell_ = cell;
}

int zakirov::CaveComponent::getDist()
{
  return dist_;
}

int zakirov::CaveComponent::setDist(int dist)
{
  dist_ = dist;
}

void zakirov::CaveComponent::visitCell()
{
  visit_ = true;
}

bool zakirov::CaveComponent::isVisited()
{
  return visit_;
}

bool zakirov::CaveComponent::isWall() const noexcept
{
  return cell_ == '#';
}

bool zakirov::CaveComponent::isPath() const noexcept
{
  return cell_ == '.';
}

bool zakirov::CaveComponent::isEntry() const noexcept
{
  return cell_ == '+';
}

bool zakirov::CaveComponent::isExit() const noexcept
{
 return (cell_ == '=' || (cell_ > 'a' && cell_ < 'z') || (cell_ > 'A' && cell_ < 'Z'));
}

std::pair< int, int > zakirov::CaveLayer::getCords() const noexcept
{
  return cords_;
}

void zakirov::CaveLayer::setCords(std::pair< int, int > cords) noexcept
{
  cords_ = cords;
}

zakirov::CaveLayer & zakirov::CaveLayer::getPath()
{
  std::queue< int > bypass;
  cave_[scopes_.first].first.visitCell();
  bypass.push(scopes_.first);
  while (!bypass.empty())
  {
    int pos = bypass.front();
    int curr_distance = cave_[pos].first.getDist();
    bypass.pop();
    for (auto i = cave_[pos].second.begin(); i != cave_[pos].second.end(); ++i)
    {
      if (!cave_[*i].first.isVisited() && !cave_[*i].first.isWall())
      {
        cave_[*i].first.setDist(curr_distance + 1);
        cave_[*i].first.visitCell();
        bypass.push(*i);
      }
    }

    if (cave_[scopes_.second].first.isVisited())
    {
      break;
    }
  }
}

bool zakirov::CaveLayer::isScopes()
{
  return scopes_.first != -1 || scopes_.second != -1;
}

char zakirov::CaveLayer::getName() const noexcept
{
  return name_;
}

void zakirov::CaveLayer::setName(char name) noexcept
{
  name_ = name;
}

void zakirov::CaveLayer::addEdge(int key1, int key2)
{
  cave_[key1].second.push_back(key2);
  cave_[key2].second.push_back(key1);
}

void zakirov::CaveLayer::addEdges(int key)
{
  if (key % cords_.first != 0)
  {
    addEdge(key, key - 1);
  }
  
  if (key - cords_.first >= 0)
  {
    addEdge(key, key - cords_.first);
  }
}

void zakirov::CaveLayer::addCell(int key, CaveComponent & cell)
{
  std::list< int > edges;
  cave_.insert({key, {cell, {}}});
  addEdges(key);
  if (cell.isEntry())
  {
    scopes_.first = key;
    cell.setDist(0);
  }
  else if (cell.isExit())
  {
    scopes_.second = key;
  }
  else
  {
    cell.setDist(-1);
  }
}

zakirov::CaveComponent & zakirov::CaveLayer::getCell(int key)
{
  return cave_[key].first;
}

std::istream & zakirov::operator>>(std::istream & in, zakirov::CaveComponent & comp_c)
{
  char element_c;
  in >> element_c;
  comp_c.setCell(element_c);
  if (!comp_c.isWall() && !comp_c.isPath() && !comp_c.isEntry() && !comp_c.isExit())
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::istream & zakirov::operator>>(std::istream & in, zakirov::CaveLayer & layer_c)
{
  Guardian guard(in);
  char name;
  int width = 0;
  int length = 0;
  in >> std::noskipws;
  in >> name >> MinorSymbol{' '};
  in >> length >> MinorSymbol{' '} >> width >> MinorSymbol{'\n'};
  layer_c.setName(name);
  layer_c.setCords({length, width});
  size_t key = 0;
  CaveComponent inserter;
  for (size_t i = 0; i < width; ++i)
  {
    in >> inserter;
    layer_c.addCell(key, inserter);
    for (size_t j = 0; j < length - 1; ++j)
    {
      in >> MinorSymbol{' '} >> inserter;
      layer_c.addCell(key, inserter);
    }

    in >> MinorSymbol{'\n'};
  }

  return in;
}
