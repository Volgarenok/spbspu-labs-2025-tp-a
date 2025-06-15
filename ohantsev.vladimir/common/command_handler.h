#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <map>
#include <iosfwd>
#include <string>
#include <functional>

namespace ohantsev
{
  class CommandHandler
  {
  public:
    virtual void operator()();

    CommandHandler(std::istream& in, std::ostream& out);
    virtual ~CommandHandler() = default;

    bool add(const std::string& name, std::function< void() > func);
    bool remove(const std::string& name);
    void run(const std::string& name);

  private:
    std::map< std::string, std::function< void() > > cmds_;

  protected:
    std::istream& in_;
    std::ostream& out_;
  };
}
#endif
