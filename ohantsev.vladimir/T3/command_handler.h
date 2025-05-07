#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include <map>
#include <iosfwd>
#include <string>
#include <functional>

namespace ohantsev
{
  template < class T >
  class CommandHandler
  {
  public:
    virtual void operator()();

    CommandHandler(T& object, std::istream& in, std::ostream& out);
    virtual ~CommandHandler() = default;

    bool add(const std::string& name, const std::function< void() >& func);
    bool remove(const std::string& name);
    void run(const std::string& name);

  private:
  std::map< std::string, std::function< void() > > cmds_;

  protected:
    T& object_;
    std::istream& in_;
    std::ostream& out_;
  };

  template < class T >
  CommandHandler< T >::CommandHandler(T& object, std::istream& in, std::ostream& out):
    cmds_(),
    object_(object),
    in_(in),
    out_(out)
  {}

  template < class T >
  void CommandHandler< T >::operator()()
  {
    std::string subcmd;
    in_ >> subcmd;
    run(subcmd);
  }

  template < class T >
  bool CommandHandler< T >::add(const std::string& name, const std::function< void() >& func)
  {
    return cmds_.emplace(name, func).second;
  }

  template < class T >
  bool CommandHandler< T >::remove(const std::string& name)
  {
    return cmds_.erase(name);
  }

  template < class T >
  void CommandHandler< T >::run(const std::string& name)
  {
    cmds_.at(name)();
  }
}
#endif
