#include "commands.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <functional>
#include <vector>
#include <utility>

namespace
{
  struct NullOstreamIterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
  {
    template < class T >
    NullOstreamIterator& operator=(const T&)
    {
      return *this;
    }
    NullOstreamIterator& operator*()
    {
      return *this;
    }
    NullOstreamIterator& operator++()
    {
      return *this;
    }
    NullOstreamIterator operator++(int)
    {
      return *this;
    }
  };

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
    int rait, pr;
    in >> playerName >> position >> rait >> pr;
    if (rait > 0 && pr > 0 && brevnov::checkPosition(position))
    {
      size_t raiting = rait;
      size_t price = pr;
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
    else
    {
      std::cerr << "Not correct parameters!\n";
    }
  }

  bool isTeamNamed(const std::pair<std::string, brevnov::Team>& teamPair, const std::string& name)
  {
    return teamPair.first == name;
  }

  void addT(std::istream& in, brevnov::League& league, const std::string& teamName)
  {
    int bud = 0;
    in >> bud;
    if (bud > 0)
    {
      size_t budget = bud;
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
    struct FreeAgentMatcherPrinter
    {
      bool operator()(const std::pair<std::string, brevnov::Player>& player) const
      {
        if (matcher(player))
        {
          out << "FA " << player.first << " " << player.second << "\n";
        }
        return false;
      }
      std::ostream& out;
      PositionMatcher& matcher;
    };
    FreeAgentMatcherPrinter printer{out, matcher};
    NullOstreamIterator null_it;
    std::transform(league.fa_.begin(), league.fa_.end(), null_it, printer);
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

  struct BestPlayerComparator
  {
    BestPlayerComparator(const SuitablePlayer& pred):
      predicate(pred)
    {}
    template < class T >
    bool operator()(const T& a, const T& b) const
    {
      bool a_match = predicate(a);
      bool b_match = predicate(b);
      if (a_match && !b_match)
      {
        return false;
      }
      if (!a_match && b_match)
      {
        return true;
      }
      return a.second.raiting_ < b.second.raiting_;
    }
    const SuitablePlayer& predicate;
  };

  void buyP(std::ostream& out, brevnov::League& league, brevnov::Team& club, size_t bud, brevnov::Position sPos)
  {
    SuitablePlayer predicate(sPos, bud, 0);
    BestPlayerComparator comp(predicate);
    auto bestPlayerIt = std::max_element(league.fa_.begin(), league.fa_.end(), comp);
    if (bestPlayerIt != league.fa_.end() && predicate(*bestPlayerIt))
    {
      club.budget_ -= bestPlayerIt->second.price_;
      out << "Bought " << bestPlayerIt->first << " " << bestPlayerIt->second << "\n";
      club.players_.insert(*bestPlayerIt);
      league.fa_.erase(bestPlayerIt);
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
      out << teamName << " " << pair.first << " " << pair.second << "\n";
      return out;
    }
    const std::string& teamName;
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
      return a.second.raiting_ < b.second.raiting_;
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
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      out << "FA " << player.first << " " << player.second << "\n";
      return true;
    }
    std::ostream& out;
  };

  struct FilteredPrinter
  {
    bool operator()(const std::pair< const std::string, brevnov::Player >& player) const
    {
      if (matcher(player))
      {
        printer(player);
        return true;
      }
      return false;
    }
    PositionMatcher& matcher;
    TeamPlayerPrinter& printer;
  };

  struct PositionFilter
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& p) const
    {
      return p.second.position_ == pos;
    }
    brevnov::Position pos;
  };

  struct RatingComparator
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& a, const std::pair<std::string, brevnov::Player>& b) const
    {
      return a.second.raiting_ < b.second.raiting_;
    }
  };

  struct PositionProcessor
  {
    PositionProcessor(brevnov::Team& t, std::ostream& o):
      team(t),
      out(o)
    {}
    int operator()(brevnov::Position pos) const
    {
      std::vector<std::pair<std::string, brevnov::Player>> players_on_pos;
      PositionFilter filter{pos};
      std::copy_if(team.players_.begin(), team.players_.end(), std::back_inserter(players_on_pos), filter);
      if (!players_on_pos.empty())
      {
        RatingComparator comp;
        auto best_player = std::max_element(players_on_pos.begin(), players_on_pos.end(), comp);
        out << best_player->first << " " << best_player->second << "\n";
        return 1;
      }
      else
      {
        out << "No player on position!\n";
        return 0;
      }
    }
    brevnov::Team& team;
    std::ostream& out;
  };

  void viewTeamP(std::ostream& out, std::string teamName, std::string pos, brevnov::League& league)
  {
    if (!brevnov::checkPosition(pos))
    {
      std::cerr << "Not correct position!\n";
      return;
    }
    brevnov::Position sPos = brevnov::definePosition(pos);
    auto findTeam = league.teams_.find(teamName);
    if (findTeam == league.teams_.end())
    {
      std::cerr << "Team not found!\n";
    }
    PositionMatcher matcher{sPos};
    TeamPlayerPrinter printer{teamName, out};
    FilteredPrinter adapter{matcher, printer};
    NullOstreamIterator null_iter;
    std::copy_if(findTeam->second.players_.begin(), findTeam->second.players_.end(), null_iter, adapter);
  }

  struct PlayerOutputter
  {
    PlayerOutputter(const std::string& name, std::ostream& os):
      printer{name, os},
      out(os)
    {}
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      printer(player);
      return false;
    }
    TeamPlayerPrinter printer;
    std::ostream& out;
  };

  struct PlayerHandler
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      team.budget_ += player.second.price_;
      league.fa_.insert(player);
      return true;
    }
    brevnov::Team& team;
    brevnov::League& league;
  };

  struct PositionBuyer
  {
    bool operator()(brevnov::Position pos) const
    {
      buyP(out, league, team, budget, pos);
      return true;
    }
    std::ostream& out;
    brevnov::League& league;
    brevnov::Team& team;
    size_t budget;
  };

  struct TeamPlayerPositionPrinter
  {
    bool operator()(const std::pair<std::string, brevnov::Player>& player) const
    {
      if (matcher(player))
      {
        out << teamName << " " << player.first << " " << player.second << "\n";
      }
      return false;
    }
    std::ostream& out;
    const std::string& teamName;
    PositionMatcher& matcher;
  };

  struct TeamProcessor
  {
    bool operator()(const std::pair<std::string, brevnov::Team>& team) const
    {
      TeamPlayerPositionPrinter printer{out, team.first, matcher};
      NullOstreamIterator null_it;
      std::transform(team.second.players_.begin(), team.second.players_.end(), null_it, printer);
      return false;
    }
    std::ostream& out;
    PositionMatcher& matcher;
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
  if (playerIt == ((soldTeamIt != league.teams_.end()) ? soldTeamIt->second.players_.end() : league.fa_.end()))
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
  int budg = 0;
  in >> budg >> pos >> teamName;
  if (!checkPosition(pos))
  {
    std::cerr << "Not correct position!\n";
    return;
  }
  if (budg <= 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  size_t bud = budg;
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
  int budg = 0;
  in >> budg >> teamName;
  if (budg <= 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  size_t bud = budg;
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
  PositionBuyer buyer{out, league, teamIt->second, bud / 6};
  std::vector<int> dummy(positions.size());
  std::transform(positions.begin(), positions.end(), dummy.begin(), buyer);
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
  PlayerHandler handler{teamIt->second, league};
  std::vector<std::pair<std::string, Player>> players(teamIt->second.players_.begin(),
    teamIt->second.players_.end());
  std::vector<int> dummy(players.size());
  std::transform(players.begin(), players.end(), dummy.begin(), handler);
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
    PlayerOutputter outputter{teamName, out};
    std::vector<std::pair<std::string, Player>> players(findTeam->second.players_.begin(),
      findTeam->second.players_.end());
    std::vector<int> dummy(players.size());
    std::transform(players.begin(), players.end(), dummy.begin(), outputter);
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
  TeamProcessor processor{out, matcher};
  std::vector<int> dummy(league.teams_.size());
  std::transform(league.teams_.begin(), league.teams_.end(), dummy.begin(), processor);
}

void brevnov::buyPlayer(std::istream& in, std::ostream& out, League& league)
{
  std::string teamName;
  int budg = 0;
  in >> budg >> teamName;
  if (budg <= 0)
  {
    std::cerr << "Not correct budget!\n";
    return;
  }
  size_t bud = budg;
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
  const std::array<Position, 6> positions = {Position::LF, Position::RF, Position::CF,
    Position::LB, Position::RB, Position::G};
  PositionProcessor processor{club, out};
  std::vector<int> dummy(positions.size());
  std::transform(positions.begin(), positions.end(), dummy.begin(), processor);
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
  viewTeamP(out, teamName, pos, league);
}

void brevnov::viewMarket(std::ostream& out, League& league)
{
  FreeAgentPrinter printer{out};
  std::transform(league.fa_.begin(), league.fa_.end(), NullOstreamIterator{}, printer);
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
