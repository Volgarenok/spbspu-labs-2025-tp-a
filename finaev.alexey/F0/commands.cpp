#include "commands.hpp"
#include <delimiter.hpp>
#include <streamGuard.hpp>
#include <limits>
#include <random>
#include <fstream>
#include <iterator>

namespace
{
  struct FileStrProcessor
  {
    std::istream& in;
    std::unordered_map< std::string, finaev::OpenningInfo >& debuts;
    std::ostream& out;

    void operator()(const std::string&)
    {
      std::string key;
      if (!(in >> key))
      {
        return;
      }
      if (debuts.find(key) != debuts.end())
      {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
      }
      finaev::StreamGuard guard(in);
      finaev::OpenningInfo inf;
      if (!(in >> inf.moves))
      {
        return;
      }
      in >> std::ws;
      in >> finaev::DelimiterIO{'"'};
      std::getline(in, inf.name, '"');
      in >> std::ws;
      in >> finaev::DelimiterIO{'"'};
      std::getline(in, inf.description, '"');
      debuts[key] = inf;
      out << "Opening " << key << " successfully added\n";
    }
  };

  struct Str
  {
    std::string content;
  };

  std::istream& operator>>(std::istream& is, Str& str)
  {
    return std::getline(is, str.content);
  }

  struct StrConvert
  {
    std::string operator()(const Str& s) const
    {
      return s.content;
    }
  };

  bool isExactMoves(const finaev::DebutMoves& f, const finaev::DebutMoves& s)
  {
    return f.moves == s.moves;
  }

  struct СompareMoves
  {
    const finaev::DebutMoves& mov;
    const finaev::globalDebuts& globalOpenings;

    bool operator()(const std::pair< std::string, bool >& base)
    {
      auto it = globalOpenings.find(base.first);
      return it != globalOpenings.end() && isExactMoves(it->second.moves, mov);
    }
  };

  struct ResultCollectorForFind
  {
    const finaev::globalDebuts& debuts;
    std::vector< std::pair< std::string, std::string > >& results;
    const finaev::DebutMoves& mov;

    void operator()(const std::pair< std::string, bool >& baseEntry) const
    {
      auto debut = debuts.find(baseEntry.first);
      if (debut != debuts.end() && debut->second.moves.containsSequence(mov.moves))
      {
        results.emplace_back(baseEntry.first, debut->second.name);
      }
    }
  };

  struct KeySorterForFind
  {
    bool operator()(const std::pair< std::string, std::string >& a, const std::pair< std::string, std::string >& b) const
    {
      return a.first < b.first;
    }
  };

  struct OutputFormatForFind
  {
    std::ostream& out;

    void operator()(const std::pair< std::string, std::string >& pair) const
    {
      static bool first = true;
      if (!first)
      {
        out << "\n";
      }
      out << "- " << pair.first << ": " << pair.second;
      first = false;
    }
  };

  struct OutputFormatForPrint
  {
    std::ostream& out;
    const finaev::globalDebuts& debuts;
    bool first = true;

    void operator()(const std::pair< std::string, bool >& base)
    {
      auto it = debuts.find(base.first);
      if (it != debuts.end())
      {
        if (!first)
        {
          out << "\n";
        }
        out << "- " << it->first << ": " << it->second.name;
        first = false;
      }
    }
  };

  struct Merge
  {
    std::unordered_map< std::string, bool >& target;
    size_t& add;
    const finaev::globalDebuts& debuts;

    void operator()(const std::pair< std::string, bool >& base)
    {
      if (debuts.find(base.first) == debuts.end())
      {
        return;
      }
      auto res = target.insert(base);
      if (res.second)
      {
        ++add;
      }
    }
  };

  struct Intersect
  {
    const std::unordered_map< std::string, bool >& base1;
    const finaev::globalDebuts& debuts;
    std::unordered_map< std::string, bool >& res;

    void operator()(const std::pair< std::string, bool >& base2)
    {
      if ((base1.find(base2.first) != base1.end()) && (debuts.find(base2.first) != debuts.end()))
      {
        res.insert(base2);
      }
    }
  };

  struct Complement
  {
    const std::unordered_map< std::string, bool >& base1;
    const finaev::globalDebuts& debuts;
    std::unordered_map< std::string, bool >& res;

    void operator()(const std::pair< std::string, bool >& base2)
    {
      if ((base1.find(base2.first) == base1.end()) && (debuts.find(base2.first) != debuts.end()))
      {
        res.insert(base2);
      }
    }
  };

  struct RemoveDebut
  {
    const std::string& key;

    void operator()(std::pair< const std::string, std::unordered_map< std::string, bool > >& base)
    {
      base.second.erase(key);
    }
  };

  struct CheckDebut
  {
    const std::string& key;
    bool& inBase;

    void operator()(const std::pair< std::string, std::unordered_map< std::string, bool > >& base)
    {
      if (base.second.find(key) != base.second.end())
      {
        inBase = true;
      }
    }
  };

  struct CountBasesWithDebut
  {
    const std::string& key;
    size_t& count;

    void operator()(const std::pair< std::string, std::unordered_map< std::string, bool > >& base)
    {
      if (base.second.find(key) != base.second.end())
      {
        ++count;
      }
    }
  };

  struct IsChessSquare
  {
    bool operator()(const std::string& sq) const
    {
      if (sq.length() != 2)
      {
        return false;
      }
      char file = tolower(sq[0]);
      char rank = sq[1];
      return file >= 'a' && file <= 'h' && rank >= '1' && rank <= '8';
    }
  };

  struct IsChessMove
  {
    IsChessSquare squareValid;

    bool operator()(const std::string& move) const
    {
      if (move.length() != 5 || move[2] != '-')
      {
        return false;
      }
      std::string from = move.substr(0, 2);
      std::string to = move.substr(3, 2);
      return squareValid(from) && squareValid(to);
    }
  };

  struct IsAllMovesValid
  {
    IsChessMove moveValid;
    bool& allValid;

    void operator()(const finaev::DebutMoves& moves)
    {
      allValid = (std::all_of(moves.moves.begin(), moves.moves.end(), moveValid));
    }
  };

  struct KeyExtract
  {
    std::vector<std::string>& keys;

    void operator()(const std::pair< std::string, bool >& pair) const
    {
      keys.push_back(pair.first);
    }
  };

  struct MovesPrint
  {
    std::ostream& out;

    void operator()(const std::string& move) const
    {
      out << move << " ";
    }
  };

  struct DebutPrint
  {
    std::ostream& out;
    const finaev::globalDebuts& debuts;
    size_t index = 1;

    void operator()(const std::string& key)
    {
      out << (index++) << ") " << debuts.at(key).name << "\n";
    }
  };
}

void finaev::loadDebutsFromFile(const std::string& filename, globalDebuts& debuts, std::ostream& out)
{
  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Error opening file");
  }
  std::vector< std::string > lines;
  std::transform(std::istream_iterator<Str>(file), std::istream_iterator< Str >(),std::back_inserter(lines), StrConvert{});
  FileStrProcessor processor{ file, debuts, out };
  std::for_each(lines.begin(), lines.end(), std::ref(processor));
}

void finaev::create_debut(std::istream& in, std::ostream& out, globalDebuts& debuts)
{
  std::string key;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) != debuts.end())
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    throw std::runtime_error("<DUPLICATE>");
  }
  StreamGuard guard(in);
  OpenningInfo inf;
  if (!(in >> inf.moves))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  in >> std::ws;
  in >> DelimiterIO{'"'};
  std::getline(in, inf.name, '"');
  in >> std::ws;
  in >> DelimiterIO{'"'};
  std::getline(in, inf.description, '"');
  debuts[key] = inf;
  out << "Openning " << key << " successfully added";
}

void finaev::create_base(std::istream& in, std::ostream& out, debutsBases& db)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (db.find(baseName) != db.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  db[baseName] = std::unordered_map<std::string, bool>();
  out << "Base " << baseName << " successfully added";
}

void finaev::add(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string baseName, debut;
  if (!(in >> baseName >> debut))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(debut) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto& base = bases[baseName];
  if (base.find(debut) != base.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  base[debut] = true;
  out << "Debut " << debut <<  " successfully added in base " << baseName;
}

void finaev::exact_find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  const auto& base = bases.at(baseName);
  СompareMoves pred{ mov, debuts };
  auto found = std::find_if(base.begin(), base.end(), pred);
  if (found != base.end())
  {
    const auto& opening = debuts.at(found->first);
    out << "Key: " << found->first << "\n";
    out << "Name: " << opening.name;
  }
  else
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
}

void finaev::find(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  DebutMoves mov;
  in >> mov;
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(baseName) == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base = bases.find(baseName);
  std::vector< std::pair< std::string, std::string > > foundOpenings;
  ResultCollectorForFind collector{ debuts, foundOpenings, mov};
  std::for_each(base->second.begin(), base->second.end(), collector);
  std::sort(foundOpenings.begin(), foundOpenings.end(), KeySorterForFind());
  if (foundOpenings.empty())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  out << "Found " << foundOpenings.size() << " openings:\n";
  std::for_each(foundOpenings.begin(), foundOpenings.end(), OutputFormatForFind{ out });
}

void finaev::print(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::string temp;
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  OutputFormatForPrint printer{ out, debuts };
  std::for_each(base->second.begin(), base->second.end(), printer);
}

void finaev::move(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string sourceBase, targetBase, key, temp;
  if (!(in >> sourceBase >> targetBase >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto srcBase = bases.find(sourceBase);
  if (srcBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto trgBase = bases.find(targetBase);
  if (trgBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto debut = srcBase->second.find(key);
  if (debut == srcBase->second.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  if (trgBase->second.find(key) != trgBase->second.end())
  {
    out << "Debut " << key << " exists in target base";
    return;
  }
  trgBase->second[key] = true;
  srcBase->second.erase(debut);
  out << "Debut " << key << " moved from " << sourceBase << " to " << targetBase;
}

void finaev::merge(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string targetBase, sourceBase, temp;
  if (!(in >> targetBase >> sourceBase))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto srcBase = bases.find(sourceBase);
  if (srcBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto trgBase = bases.find(targetBase);
  if (trgBase == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  size_t add = 0;
  Merge merger{ trgBase->second, add, debuts };
  std::for_each(srcBase->second.begin(), srcBase->second.end(), merger);
  out << "Merged " << sourceBase << " into " << targetBase << ": "<< add << " debuts added";
}

void finaev::intersect(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string newBaseName, base1Name, base2Name, temp;
  if (!(in >> newBaseName >> base1Name >> base2Name))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(newBaseName) != bases.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  auto base1 = bases.find(base1Name);
  if (base1 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base2 = bases.find(base2Name);
  if (base2 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  std::unordered_map< std::string, bool > intersectionBase;
  Intersect inter{ base1->second, debuts, intersectionBase };
  std::for_each(base2->second.begin(), base2->second.end(), inter);
  bases[newBaseName] = std::move(intersectionBase);
  out << "Created base " << newBaseName << " with " << bases[newBaseName].size();
  out << " openings (intersection of " << base1Name << " and " << base2Name << ")";
}

void finaev::complement(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string newBaseName, base1Name, base2Name, temp;
  if (!(in >> newBaseName >> base1Name >> base2Name))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (bases.find(newBaseName) != bases.end())
  {
    throw std::runtime_error("<DUPLICATE>");
  }
  auto base1 = bases.find(base1Name);
  if (base1 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto base2 = bases.find(base2Name);
  if (base2 == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  std::unordered_map< std::string, bool > complementionBase;
  Complement comp{ base2->second, debuts, complementionBase };
  std::for_each(base1->second.begin(), base1->second.end(), comp);
  bases[newBaseName] = std::move(complementionBase);
  out << "Created base " << newBaseName << " with " << bases[newBaseName].size();
  out << " openings (complemention of " << base2Name << " from " << base1Name << ")";
}

void finaev::remove(std::istream& in, std::ostream& out, const globalDebuts& debuts, debutsBases& bases)
{
  std::string baseName, key, temp;
  if (!(in >> baseName >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  auto debut = base->second.find(key);
  if (debut == base->second.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  base->second.erase(debut);
  out << "Debut " << key << " removed from base " << baseName;
}

void finaev::force_delete(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  RemoveDebut remover{ key };
  std::for_each(bases.begin(), bases.end(), remover);
  debuts.erase(key);
  out << "Debut " << key << " completely deleted from system debuts";
}

void finaev::safe_delete(std::istream& in, std::ostream& out, globalDebuts& debuts, const debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (debuts.find(key) == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  bool inBase = false;
  CheckDebut checker{ key, inBase };
  std::for_each(bases.begin(), bases.end(), checker);
  if (inBase)
  {
    out << "Cannot delete " << key << " exists in one or more bases";
    return;
  }
  debuts.erase(key);
  out << "Debut " << key << " safely deleted from system";
}

void finaev::delete_debut(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto debut = debuts.find(key);
  if (debut == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  size_t count = 0;
  CountBasesWithDebut counter{ key, count };
  std::for_each(bases.begin(), bases.end(), std::ref(counter));
  if (counter.count > 0)
  {
    out << "Are you sure? The debut is contained in " << count << " bases [y/n]: ";
    char answer;
    in >> answer;
    if (answer != 'y' && answer != 'Y')
    {
      throw std::runtime_error("Deletion cancelled");
    }
    RemoveDebut remover{ key };
    std::for_each(bases.begin(), bases.end(), remover);
  }
  debuts.erase(debut);
  out << "Debut " << key << (count > 0 ? " completely deleted from system debuts" : " safely deleted from system");
}

void finaev::validate(std::istream& in, std::ostream& out, const globalDebuts& debuts)
{
  std::string key, temp;
  if (!(in >> key))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto debut = debuts.find(key);
  if (debut == debuts.end())
  {
    throw std::runtime_error("<OPENNING_NOT_FOUND>");
  }
  IsChessSquare squareValid;
  IsChessMove moveValid{ squareValid };
  bool isValidate = false;
  IsAllMovesValid validator{ moveValid, isValidate };
  validator(debut->second.moves);
  out << key << (isValidate ? " - ok" : " - uncorrect");
}

void finaev::guess(std::istream& in, std::ostream& out, const globalDebuts& debuts, const debutsBases& bases)
{
  std::string baseName, temp;
  if (!(in >> baseName))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::getline(in, temp);
  if (!temp.empty())
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  auto base = bases.find(baseName);
  if (base == bases.end())
  {
    throw std::runtime_error("<NO_DEBUT_BASE>");
  }
  std::vector< std::string > debutsInBase;
  KeyExtract extractor{ debutsInBase };
  std::for_each(base->second.begin(), base->second.end(), extractor);
  if (debutsInBase.size() < 4)
  {
    throw std::runtime_error("Not enough debuts in base");
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(debutsInBase.begin(), debutsInBase.end(), gen);
  debutsInBase.resize(4);
  std::uniform_int_distribution<> distr(0, 3);
  const size_t correctIndex = distr(gen);
  const std::string& correctKey = debutsInBase[correctIndex];
  const auto& correctDebut = debuts.at(correctKey);
  out << "Guess debut:\n";
  std::for_each(correctDebut.moves.moves.begin(), correctDebut.moves.moves.end(), MovesPrint{ out });
  out << "\n";
  std::for_each(debutsInBase.begin(), debutsInBase.end(), DebutPrint{out, debuts});
  out << "Choise 1-4: ";
  out.flush();
  int choice;
  if (!(in >> choice) || choice < 1 || choice > 4)
  {
    throw std::runtime_error("Invalid choice");
  }
  const std::string& selectedKey = debutsInBase[choice - 1];
  if (selectedKey == correctKey)
  {
    out << "True! is " << correctDebut.name << ". " << correctDebut.description;
  }
  else
  {
    out << "Not true! is " << correctDebut.name << ". " << correctDebut.description;
  }
}

finaev::mapOfCommands finaev::createCommandsHandler(std::istream& in, std::ostream& out, globalDebuts& debuts, debutsBases& bases)
{
  mapOfCommands commands;
  commands["create_debut"] = std::bind(finaev::create_debut, std::ref(in), std::ref(out), std::ref(debuts));
  commands["create_base"] = std::bind(finaev::create_base, std::ref(in), std::ref(out), std::ref(bases));
  commands["add"] = std::bind(finaev::add, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["exact_find"] = std::bind(finaev::exact_find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["find"] = std::bind(finaev::find, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["print"] = std::bind(finaev::print, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  commands["move"] = std::bind(finaev::move, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["merge"] = std::bind(finaev::merge, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["intersect"] = std::bind(finaev::intersect, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["complement"] = std::bind(finaev::complement, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["remove"] = std::bind(finaev::remove, std::ref(in), std::ref(out), std::cref(debuts), std::ref(bases));
  commands["force_delete"] = std::bind(finaev::force_delete, std::ref(in), std::ref(out), std::ref(debuts), std::ref(bases));
  commands["safe_delete"] = std::bind(finaev::safe_delete, std::ref(in), std::ref(out), std::ref(debuts), std::cref(bases));
  commands["delete"] = std::bind(finaev::delete_debut, std::ref(in), std::ref(out), std::ref(debuts), std::ref(bases));
  commands["validate"] = std::bind(finaev::validate, std::ref(in), std::ref(out), std::cref(debuts));
  commands["guess"] = std::bind(finaev::guess, std::ref(in), std::ref(out), std::cref(debuts), std::cref(bases));
  return commands;
}
