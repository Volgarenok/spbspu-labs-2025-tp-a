#include "args-parser.hpp"
#include <cassert>
#include <ostream>

namespace kizhin {
  std::ostream& printUsage(std::ostream&, const char*);
}

kizhin::Args kizhin::parseArgs(const int argc, const char* const* const argv,
    std::ostream& usageOut)
{
  assert(argv && argv[0] && "argv is null or program name is missing");
  assert(argc > 0 && "argc must be positive");
  if (argc != 2 || argv[1][0] == '\0') {
    printUsage(usageOut, argv[0]) << '\n';
    std::exit(1);
  }
  return Args{ argv[1] };
}

std::ostream& kizhin::printUsage(std::ostream& out, const char* executablePath)
{
  assert(executablePath && "printUsage: nullptr instead of path");
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  return out << "Usage: " << executablePath << " <filename>";
}

