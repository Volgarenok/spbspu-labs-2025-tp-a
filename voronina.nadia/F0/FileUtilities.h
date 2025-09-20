#ifndef FILE_UTILITIES
#define FILE_UTILITIES

#include <string>

namespace voronina
{
  std::string readFileContents(std::string const& filename);
  void writeInFile(const std::string& filename, const std::string& text);
  std::streamsize getFileSize(const std::string& fileName);
}

#endif
