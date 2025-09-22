#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <map>
#include <functional>
#include <iosfwd>
namespace nikonov {
  using str = std::string;
  class Storage;
  void getCommands(std::map< std::string, std::function< void(Storage&, std::istream&, std::ostream&) > >& commands);
  void compressText(Storage& storage, std::istream& in, std::ostream& out);
  void decompressText(Storage& storage, std::istream& in, std::ostream& out);
  void createEncoding(Storage& storage, std::istream& in, std::ostream& out);
  void applyEncoding(Storage& storage, std::istream& in, std::ostream& out);
  void compareEncodings(Storage& storage, std::istream& in, std::ostream& out);
  void showEncoding(Storage& storage, std::istream& in, std::ostream& out);
  void textStats(Storage& storage, std::istream& in, std::ostream& out);
  void testCompression(Storage& storage, std::istream& in, std::ostream& out);
  void loadTextFromFile(Storage& storage, std::istream& in, std::ostream& out);
  void readTextFromStdin(Storage& storage, std::istream& in, std::ostream& out);
  void uploadText(Storage& storage, std::istream& in, std::ostream& out);
  void getEncodingFromFile(Storage& storage, std::istream& in, std::ostream& out);
  void uploadEncoding(Storage& storage, std::istream& in, std::ostream& out);
}
#endif
