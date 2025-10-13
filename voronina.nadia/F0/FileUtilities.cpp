#include "FileUtilities.h"

#include <fstream>

namespace voronina
{
  std::string readFileContents(std::string const& filename)
  {
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    if (!ifs.is_open())
    {
      static const char* prefix = "Ошибка при открытии файла: ";
      static const char* postfix = ". Проверьте существование такого файла";
      throw std::invalid_argument(prefix + filename + postfix);
    }
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::string result;
    result.resize(static_cast< std::size_t >(size));
    if (!ifs.read(&result[0], size))
    {
      throw std::runtime_error("Ошибка при чтении файла: " + filename);
    }
    return result;
  }

  void writeInFile(std::string const& filename, std::string const& text)
  {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
      throw std::invalid_argument("Ошибка при открытии файла для записи: " + filename);
    }
    ofs << text;
  }

  std::streamsize getFileSize(std::string const& fileName)
  {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
      throw std::invalid_argument("Не удалось открыть файл " + fileName);
    }
    return file.tellg();
  }
}
