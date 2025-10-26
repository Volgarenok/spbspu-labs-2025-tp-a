#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <limits>
#include "Wrapper.hpp"

int main()
{
  using namespace milchevskiy;
  setlocale(LC_ALL, "Russian");

  GraphSystem graphSystem;
  std::map<std::string, std::function<void()>> cmds;

  cmds["create"] = std::bind(createWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["isEmpty"] = std::bind(isEmptyWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["searchNode"] = std::bind(searchNodeWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["searchArc"] = std::bind(searchArcWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["insertNode"] = std::bind(insertNodeWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["insertArc"] = std::bind(insertArcWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["deleteNode"] = std::bind(deleteNodeWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["print"] = std::bind(printWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["clear"] = std::bind(clearWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["isCyclical"] = std::bind(isCyclicalWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["shortestPath"] = std::bind(shortestPathWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));
  cmds["topologicalSort"] = std::bind(topologicalSortWrapper, std::ref(graphSystem), std::ref(std::cin), std::ref(std::cout));

  std::string command;

  while (std::cin >> command)
  {
    try
    {
      auto it = cmds.find(command);
      if (it != cmds.end())
      {
        it->second();
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "<ERROR: " << e.what() << ">\n";
    }
    catch (...)
    {
      std::cout << "<UNKNOWN ERROR>\n";
    }

    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
