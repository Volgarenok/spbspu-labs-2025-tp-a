#include "commands.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <functional>
#include <vector>
#include <utility>

namespace
{

  struct PlayerNameComparator
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& p) const
    {
      return p.first == name;
    }
    const std::string& name;
  };

  void addPlayer(std::istream& in, brevnov::League& league, std::string teamName)
  {
    std::string playerName, position;
    size_t raiting, price;
    in >> playerName >> position >> raiting >> price;
    if (raiting > 0 && price > 0 && brevnov::checkPosition(position))
    {
      auto teamIt = league.teams_.find(teamName);
      auto& targetMap = (teamIt != league.teams_.end()) ? teamIt->second.players_ : league.fa_;
      PlayerNameComparator comp{playerName};
      bool exists = std::find_if(targetMap.begin(), targetMap.end(), comp) != targetMap.end();
      if (!exists)
      {
        targetMap.emplace(playerName, brevnov::Player(position, raiting, price));
      }
      else
      {
        std::cerr << "Player already exist!\n";
      }
    }
  }

  bool isTeamNamed(const std::pair<std::string, brevnov::Team>& teamPair, const std::string& name)
  {
    return teamPair.first == name;
  }

  void addT(std::istream& in, brevnov::League& league, const std::string& teamName)
  {
    size_t budget = 0;
    in >> budget;
    if (budget > 0)
    {
      bool teamExists = std::any_of(league.teams_.begin(), league.teams_.end(),
        std::bind(isTeamNamed, std::placeholders::_1, teamName));
      if (!teamExists)
      {
        league.teams_.emplace(teamName, brevnov::Team(budget));
      }
      else
      {
        std::cerr << "Team already exists!\n";
      }
    }
    else
    {
      std::cerr << "Not correct budget!\n";
    }
  }

  struct PositionMatcher
  {
    PositionMatcher(brevnov::Position pos):
      targetPos(pos)
    {}
    bool operator()(const std::pair<std::string, brevnov::Player>& playerPair) const
    {
      return playerPair.second.position_ == targetPos;
    }
    brevnov::Position targetPos;
  };

  struct PositionNotMatcher
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      return !matcher(player);
    }
    PositionMatcher matcher;
  };

  brevnov::Position viewM(std::istream& in, std::ostream& out, brevnov::League& league)
  {
    std::string pos;
    in >> pos;
    if (!brevnov::checkPosition(pos))
    {
      std::cerr << "Not correct position!\n";
      throw std::logic_error("Not correct position!");
    }
    brevnov::Position sPos = brevnov::definePosition(pos);
    PositionMatcher matcher(sPos);
    auto it = std::find_if(league.fa_.begin(), league.fa_.end(), matcher);
    while (it != league.fa_.end())
    {
      out << "FA " << it->first << " " << it->second << "\n";
      it = std::find_if(std::next(it), league.fa_.end(), matcher);
    }
    return sPos;
  }

  struct SuitablePlayer
  {
    SuitablePlayer(brevnov::Position pos, size_t budget, size_t currentBestRating):
      requiredPos(pos),
      maxPrice(budget),
      minRating(currentBestRating)
    {}
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      const brevnov::Player& p = player.second;
      return p.position_ == requiredPos && p.price_ <= maxPrice && p.raiting_ > minRating;
    }
    brevnov::Position requiredPos;
    size_t maxPrice;
    size_t minRating;
  };

  void buyP(std::ostream& out, brevnov::League& league, brevnov::Team& club, size_t bud, brevnov::Position sPos)
  {
    size_t bestRating = 0;
    auto bestPlayer = league.fa_.end();
    for (auto it = league.fa_.begin(); it != league.fa_.end(); ++it)
    {
      SuitablePlayer predicate(sPos, bud, bestRating);
      if (predicate(*it))
      {
        bestRating = it->second.raiting_;
        bestPlayer = it;
      }
    }
    if (bestPlayer != league.fa_.end())
    {
      club.budget_ -= bestPlayer->second.price_;
      out << "Bought " << bestPlayer->first << " " << bestPlayer->second << "\n";
      club.players_.insert(*bestPlayer);
      league.fa_.erase(bestPlayer);
    }
    else
    {
      out << "Player not found!\n";
    }
  }

  struct TeamFinder
  {
    bool operator()(const std::pair<std::string, brevnov::Team>& teamPair) const
    {
      return teamPair.first == teamName;
    }
    const std::string& teamName;
  };

  struct TeamPlayerPrinter
  {
    std::ostream& operator()(const std::pair<const std::string, brevnov::Player>& pair) const
    {
      out << teamName << " " << pair.first << " " << pair.second;
      return out;
    }
    std::string& teamName;
    std::ostream& out;
  };

  struct PlayerComparator
  {
    bool operator()(const std::pair<const std::string, brevnov::Player>& a, const std::pair<const std::string, brevnov::Player>& b) const
    {
      bool a_affordable = (a.second.price_ <= budget);
      bool b_affordable = (b.second.price_ <= budget);
      if (a_affordable && !b_affordable)
      {
        return true;
      }
      if (!a_affordable && b_affordable)
      {
        return false;
      }
      return a.second.raiting_ > b.second.raiting_;
    }
    size_t budget;
  };

  struct PositionRaitingComparator
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& a, const std::pair<std::string, brevnov::Player>& b) const
    {
      if (a.second.position_ != required_pos && b.second.position_ == required_pos)
      {
        return false;
      }
      if (a.second.position_ == required_pos && b.second.position_ != required_pos)
      {
        return true;
      }
      return a.second.raiting_ < b.second.raiting_;
    }
    brevnov::Position required_pos;
  };

  struct FreeAgentPrinter
  {
    void operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      out << "FA " << player.first << " " << player.second << "\n";
    }
    std::ostream& out;
  };
}

bool brevnov::checkPosition(std::string pos)
{
  return (pos == "CF" || pos == "RF" || pos == "LF" || pos == "RB" || pos == "LB" || pos == "G");
}

void brevnov::input(std::istream& in, brevnov::League& league)
{
  std::string teamName;
  while (in >> teamName)
  {
    addT(in, league, teamName);
    if (in.get() == '\n')
    {
      break;
    }
  }
  while (in >> teamName)
  {
    addPlayer(in, league, teamName);
  }
}

void brevnov::addNewPlayer(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  addPlayer(in, league, teamName);
}

void brevnov::addTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  addT(in, league, teamName);
}

void brevnov::deleteTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  auto clubFind = league.teams_.find(teamName);
  if (clubFind != league.teams_.end())
  {
    std::copy(clubFind->second.players_.begin(), clubFind->second.players_.end(),
      std::inserter(league.fa_, league.fa_.end()));
    clubFind->second.players_.clear();
    league.teams_.erase(clubFind);
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::endCareer(std::istream& in, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto teamIt = league.teams_.find(teamName);
  if (teamIt != league.teams_.end())
  {
    if (teamIt->second.players_.erase(playerName) == 0)
    {
      std::cerr << "Player not found in team!\n";
    }
  }
  else
  {
    if (league.fa_.erase(playerName) == 0)
    {
      std::cerr << "Player not found anywhere!\n";
    }
  }
}

void brevnov::updateRating(std::istream& in, League& league)
{
  std::string teamName, playerName;
  int raiting;
  in >> teamName >> playerName >> raiting;
  if (raiting <= 0)
  {
    std::cerr << "Not correct raiting!\n";
    return;
  }
  auto teamIt = league.teams_.find(teamName);
  if (teamIt != league.teams_.end())
  {
    auto playerIt = teamIt->second.players_.find(playerName);
    if (playerIt != teamIt->second.players_.end())
    {
      playerIt->second.raiting_ = raiting;
      return;
    }
  }
  auto faIt = league.fa_.find(playerName);
  if (faIt != league.fa_.end())
  {
    faIt->second.raiting_ = raiting;
    return;
  }
  std::cerr << "Player not found!\n";
}

void brevnov::transfer(std::istream& in, std::ostream& out, League& league)
{
  std::string buyTeamName, soldTeamName, playerSold;
  in >> playerSold >> buyTeamName >> soldTeamName;
  auto buyTeamIt = league.teams_.find(buyTeamName);
  if (buyTeamIt == league.teams_.end())
  {
    std::cerr << "BuyTeam not found!\n";
    return;
  }
  auto soldTeamIt = league.teams_.find(soldTeamName);
  auto playerIt = (soldTeamIt != league.teams_.end()) ? soldTeamIt->second.players_.find(playerSold)
    : league.fa_.find(playerSold);
  if (playerIt == ((soldTeamIt != league.teams_.end()) ? soldTeamIt->second.players_.end()
    : league.fa_.end()))
  {
    std::cerr << "Player not found!\n";
    return;
  }
  if (buyTeamIt->second.budget_ < playerIt->second.price_)
  {
    std::cerr << "Not enough budget for transfer!\n";
    return;
  }
  if (soldTeamIt != league.teams_.end())
  {
    out << "Successful transfer: " << playerSold << " " << playerIt->second << " ";
    out << soldTeamName << " -> " << buyTeamName << "\n";
    buyTeamIt->second.budget_ -= playerIt->second.price_;
    soldTeamIt->second.budget_ += playerIt->second.price_;
    buyTeamIt->second.players_.insert(*playerIt);
    soldTeamIt->second.players_.erase(playerIt);
  }
  else
  {
    buyTeamIt->second.budget_ -= playerIt->second.price_;
    buyTeamIt->second.players_.insert(*playerIt);
    league.fa_.erase(playerIt);
  }
}

void brevnov::buyPosition(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, pos;
  size_t bud = 0;
  in >> bud >> pos >> teamName;
  if (!checkPosition(pos))
  {
    std::cerr << "Not correct position!\n";
    return;
  }
  if (bud == 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  TeamFinder teamFinder{teamName};
  auto teamIt = std::find_if(league.teams_.begin(), league.teams_.end(), teamFinder);
  if (teamIt == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  if (teamIt->second.budget_ < bud)
  {
    std::cerr << "Team have not enough money!\n";
    return;
  }
  buyP(out, league, teamIt->second, bud, definePosition(pos));
}

void brevnov::buyTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  size_t bud = 0;
  in >> bud >> teamName;
  if (bud == 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  auto teamIt = league.teams_.find(teamName);
  if (teamIt == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  if (teamIt->second.budget_ < bud)
  {
    std::cerr << "Team have not enough money!\n";
    return;
  }
  const std::array<Position, 6> positions = {Position::LF, Position::RF,
    Position::CF,Position::LB, Position::RB, Position::G};
  for (auto it = positions.begin(); it != positions.end(); ++it)
  {
    buyP(out, league, teamIt->second, bud / 6, *it);
  }
}

void brevnov::soldPlayer(std::istream& in, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto teamIt = league.teams_.find(teamName);
  if (teamIt == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  auto playerIt = teamIt->second.players_.find(playerName);
  if (playerIt == teamIt->second.players_.end())
  {
    std::cerr << "Player not found!\n";
    return;
  }
  teamIt->second.budget_ += playerIt->second.price_;
  league.fa_.insert(*playerIt);
  teamIt->second.players_.erase(playerIt);
}

void brevnov::soldTeam(std::istream& in, League& league)
{
  std::string teamName;
  in >> teamName;
  auto teamIt = league.teams_.find(teamName);
  if (teamIt == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  for (auto& player : teamIt->second.players_)
  {
    teamIt->second.budget_ += player.second.price_;
    league.fa_.insert(player);
  }
  teamIt->second.players_.clear();
}

void brevnov::deposit(std::istream& in, League& league)
{
  std::string teamName;
  int dep;
  in >> dep >> teamName;
  if (std::less_equal<int>{}(dep, 0))
  {
    std::cerr << "Incorrect deposit!\n";
    return;
  }
  TeamFinder finder{teamName};
  auto teamIt = std::find_if(league.teams_.begin(), league.teams_.end(), finder);
  if (teamIt != league.teams_.end())
  {
    teamIt->second.budget_ += dep;
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  in >> teamName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    TeamPlayerPrinter printer{teamName, out};
    auto it = findTeam->second.players_.begin();
    while (it != findTeam->second.players_.end())
    {
      printer(*it);
      out << "\n";
      ++it;
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewPosition(std::istream& in, std::ostream& out, League& league)
{
  Position sPos = Position::CF;
  try
  {
    sPos = viewM(in, out, league);
  }
  catch(const std::logic_error& e)
  {
    return;
  }
  PositionMatcher matcher{sPos};
  for (auto& team : league.teams_)
  {
    for (auto& player : team.second.players_)
    {
      if (matcher(player))
      {
        out << team.first << " " << player.first << " " << player.second << "\n";
      }
    }
  }
}

void brevnov::buyPlayer(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  size_t bud = 0;
  in >> bud >> teamName;
  if (bud == 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  auto findTeam = league.teams_.find(teamName);
  if (findTeam == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  Team& club = findTeam->second;
  if (club.budget_ < bud)
  {
    std::cerr << "Team have not enough money!\n";
    return;
  }
  if (league.fa_.empty())
  {
    out << "Player not found!\n";
    return;
  }
  PlayerComparator comp{bud};
  auto maxpl = std::max_element(league.fa_.begin(), league.fa_.end(), comp);
  if (maxpl != league.fa_.end() && maxpl->second.price_ <= bud)
  {
    club.budget_ -= maxpl->second.price_;
    out << "Bought " << maxpl->first << " " << maxpl->second << "\n";
    club.players_.insert(*maxpl);
    league.fa_.erase(maxpl);
  }
  else
  {
    out << "Player not found!\n";
  }
}

void brevnov::startTeam(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  in >> teamName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam == league.teams_.end())
  {
    std::cerr << "Team not found!\n";
    return;
  }
  Team& club = findTeam->second;
  for (int i = 0; i < 6; i++)
  {
    Position current_pos = static_cast<Position>(i);
    PositionRaitingComparator comp{current_pos};
    auto max_player = std::max_element(club.players_.begin(), club.players_.end(), comp);
    if (max_player != club.players_.end() && max_player->second.position_ == current_pos)
    {
      out << max_player->first << " " << max_player->second << "\n";
    }
    else
    {
      out << "Not player on this position!\n";
    }
  }
}

void brevnov::viewPlayer(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, playerName;
  in >> teamName >> playerName;
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    Team& club = findTeam->second;
    PlayerNameComparator comp{playerName};
    auto playerIt = std::find_if(club.players_.begin(), club.players_.end(), comp);
    if (playerIt != club.players_.end())
    {
      out << teamName << " " << playerIt->first << " " << playerIt->second << "\n";
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
  else
  {
    PlayerNameComparator comp{playerName};
    auto faPlayerIt = std::find_if(league.fa_.begin(), league.fa_.end(), comp);
    if (faPlayerIt != league.fa_.end())
    {
      out << "FA " << faPlayerIt->first << " " << faPlayerIt->second << "\n";
    }
    else
    {
      std::cerr << "Player not found!\n";
    }
  }
}

void brevnov::viewTeamPosition(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName, pos;
  in >> teamName >> pos;
  if (!checkPosition(pos))
  {
    std::cerr << "Not correct position!\n";
    return;
  }
  Position sPos = definePosition(pos);
  auto findTeam = league.teams_.find(teamName);
  if (findTeam != league.teams_.end())
  {
    PositionMatcher matcher{sPos};
    TeamPlayerPrinter printer{teamName, out};
    auto it = findTeam->second.players_.begin();
    while (it != findTeam->second.players_.end())
    {
      if (matcher(*it))
      {
        printer(*it);
      }
      ++it;
    }
  }
  else
  {
    std::cerr << "Team not found!\n";
  }
}

void brevnov::viewMarket(std::ostream& out, League& league)
{
  FreeAgentPrinter printer{out};
  auto it = league.fa_.begin();
  while (it != league.fa_.end())
  {
    printer(*it);
    ++it;
  }
}

void brevnov::viewMarketPosition(std::istream& in, std::ostream& out, League& league)
{
  try
  {
    viewM(in, out, league);
  }
  catch(const std::logic_error& e)
  {
    return;
  }
}
