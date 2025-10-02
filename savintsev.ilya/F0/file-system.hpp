#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP
#include <string>
#include "project-body.hpp"

namespace savintsev
{
  std::string get_filename(const std::string & filename);
  std::string get_filename_wext(const std::string & filename);

  bool has_savi_extension(const std::string & filename);
  bool validate_savi_file(const std::string & filename);

  void write_savi_file(const std::string & filename, Project & proj);
  void read_savi_file(const std::string & filename, Projects & projs);

  void cleanup_projects_with_backup(Projects & projects);
  void cleanup_projects_without_backup(Projects & projects);
}

#endif
