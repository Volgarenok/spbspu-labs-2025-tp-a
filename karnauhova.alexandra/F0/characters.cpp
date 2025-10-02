#include "characters.hpp"
#include <functional>
#include <map>
#include <random>
#include <iterator>
#include <iostream>
#include <delimiter.hpp>

karnauhova::Character::Character():
  position(0),
  name_("defolt"),
  hp_(0),
  total_hp_(0),
  atk1_(0),
  atk2_(0),
  story_("defolt"),
  philosophy_("defolt"),
  quote_win_("defolt"),
  quote_lose_("defolt")
{}

karnauhova::Character::Character(std::string name, int hp, int atk1, int atk2):
  position(0),
  name_(name),
  hp_(hp),
  total_hp_(hp),
  atk1_(atk1),
  atk2_(atk2),
  story_("defolt"),
  philosophy_("defolt"),
  quote_win_("defolt"),
  quote_lose_("defolt")
{}

int karnauhova::Character::attack()
{
    std::map< int, int > cmds;
    cmds[1] = attackHead();
    cmds[2] = attackBody();
    cmds[3] = attackArm();
    cmds[4] = attackLeg();
    try
    {
      return cmds.at(position);
    }
    catch (...)
    {
      throw std::logic_error("Error: It's not the turn to attack");
    }
}

int karnauhova::Character::attackHead()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 8)
  {
    return atk2_;
  }
  else if (x > 5)
  {
    return atk2_ - 1;
  }
  return atk2_ - 2;
}

int karnauhova::Character::attackBody()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 9)
  {
    return atk2_ - 1;
  }
  else if (x > 5)
  {
    return atk1_ + 2;
  }
  return atk1_ + 1;
}

int karnauhova::Character::attackArm()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 6)
  {
    return atk1_ + 1;
  }
  else if (x > 2)
  {
    return atk1_;
  }
  return 0;
}

int karnauhova::Character::attackLeg()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(1, 10);
  int x = dist(gen);
  if (x > 7)
  {
    return atk1_ + 2;
  }
  else if (x > 3)
  {
    return atk1_ + 1;
  }
  return atk1_;
}

void karnauhova::Character::takePhilosophy(std::string philosophy)
{
  philosophy_ = philosophy;
}

void  karnauhova::Character::takeStory(std::string story)
{
  story_ = story;
}

void karnauhova::Character::takeQuotes(std::string quote_win, std::string quote_lose)
{
  quote_win_ = quote_win;
  quote_lose_ = quote_lose;
}

bool karnauhova::Character::isLost()
{
  return total_hp_ <= 0;
}

void karnauhova::Character::damage(int size_damage)
{
  total_hp_ -= size_damage;
}

int karnauhova::Character::volumeHp() const
{
  return total_hp_;
}

int karnauhova::Character::baseHp() const
{
  return hp_;
}

std::string karnauhova::Character::getName() const
{
  return name_;
}

std::istream& karnauhova::operator>>(std::istream& in, Character& pol)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  using del = DelimiterIO;
  std::string name;
  in >> name;
  int hp = 0, atk1 = 0, atk2 = 0;
  in >> hp >> atk1 >> del{'-'} >> atk2;
  in >> std::ws;
  std::string story, philosophy, quote_win, quote_lose;
  std::getline(in, story);
  std::getline(in, philosophy);
  std::getline(in, quote_win);
  std::getline(in, quote_lose);
  if (!in)
  {
    return in;
  }
  Character temp(name, hp, atk1, atk2);
  temp.takeStory(story);
  temp.takePhilosophy(philosophy);
  temp.takeQuotes(quote_win, quote_lose);
  pol = std::move(temp);
  return in;
}

void karnauhova::Character::aboutCharacter() const
{
  std::cout << name_ << " " << hp_ << " hp  " << atk1_ << "-" << atk2_ << " atk";
}

std::string karnauhova::Character::getStory() const
{
  return story_;
}

std::string karnauhova::Character::getPhilosophy() const
{
  return philosophy_;
}

std::string karnauhova::Character::getWinQuote() const
{
  return quote_win_;
}

std::string karnauhova::Character::getLoseQuote() const
{
  return quote_lose_;
}

void karnauhova::Character::recoveryHp()
{
  total_hp_ = hp_;
}

bool karnauhova::Character::operator==(const Character& other) const
{
  return name_ == other.name_ && hp_ == other.hp_ && atk1_ == other.atk1_ && atk2_ == other.atk2_;
}

std::string karnauhova::Character::getAtk() const
{
  return std::to_string(atk1_) + "-" + std::to_string(atk2_);
}

std::ostream& karnauhova::operator<<(std::ostream& out, const Character& character)
{
  out << character.getName() << " " << character.baseHp() << " " << character.getAtk() << "\n";
  out << character.getStory() << "\n";
  out << character.getPhilosophy() << "\n";
  out << character.getWinQuote() << "\n";
  out << character.getLoseQuote();
  return out;
}
