#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <limits>
#include <shape-utils.hpp>
#include "project-body.hpp"
#include "project-cmds.hpp"
#include "confirmation-src.hpp"
#include "file-system.hpp"

int main(int argc, char * argv[])
{
  using namespace savintsev;

  std::cout << "Starting...\n";

  Projects projects;

  for (int i = 1; i < argc; ++i)
  {
    try
    {
      read_savi_file(argv[i], projects);
    }
    catch (const std::runtime_error & e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  std::cout << "Welcome to the CMD-PAINT\n";

  std::map< std::string, std::function< void() > > cmds;

  cmds["open"] = std::bind(open, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["close"] = std::bind(close, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["create"] = std::bind(create, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save"] = std::bind(save, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save_as"] = std::bind(save_as, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["save_all"] = std::bind(save_all, std::ref(std::cout), std::ref(projects));
  cmds["copy"] = std::bind(copy_proj_or_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["merge"] = std::bind(merge, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["rename"] = std::bind(rename_proj_or_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["render"] = std::bind(render, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["rectangle"] = std::bind(create_rectangle, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["complexquad"] = std::bind(create_complexquad, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["concave"] = std::bind(create_concave, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["delete"] = std::bind(delete_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["insert"] = std::bind(insert_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["swap"] = std::bind(swap_shapes, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["sf"] = std::bind(sf, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["sf_above"] = std::bind(sf_above, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["sf_below"] = std::bind(sf_below, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["move_abs"] = std::bind(move_abs, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["move_rel"] = std::bind(move_rel, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["move_all"] = std::bind(move_all, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["scale"] = std::bind(scale_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["array"] = std::bind(array_shapes, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["reverse"] = std::bind(reverse_project, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["clear"] = std::bind(clear_project, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["projects"] = std::bind(print, std::ref(std::cout), std::ref(projects));
  cmds["layers"] = std::bind(print_shapes, std::ref(std::cin), std::ref(std::cout), std::ref(projects));
  cmds["info"] = std::bind(print_info_about_shape, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  cmds["exit"] = std::bind(exit_program, std::ref(std::cin), std::ref(std::cout), std::ref(projects));

  for (std::string command; std::cin >> command;)
  {
    try
    {
      cmds.at(command)();
      if (command == "exit")
      {
        cleanup_projects_without_backup(projects);
        return 0;
      }
    }
    catch (const std::out_of_range &)
    {
      std::cout << "There is no such command or project\n";
    }
    catch (const std::runtime_error & e)
    {
      std::cout << e.what() << '\n';
    }
    catch (...)
    {
      cleanup_projects_with_backup(projects);
      std::cerr << "Unknown Error. Open data were saved to backup files. Exiting.\n";
      return 1;
    }

    if (std::cin.fail())
    {
      std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
    }
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  save_all(std::cout, projects);
  cleanup_projects_without_backup(projects);
  return 0;
}
