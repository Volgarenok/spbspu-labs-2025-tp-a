#ifndef PROJECT_CMDS_HPP
#define PROJECT_CMDS_HPP
#include <iostream>
#include "project-body.hpp"

namespace savintsev
{
  void open(std::istream & in, std::ostream & out, Projects & projs);
  void close(std::istream & in, std::ostream & out, Projects & projs);
  void create(std::istream & in, std::ostream & out, Projects & projs);
  void copy_proj_or_shape(std::istream & in, std::ostream & out, Projects & projs);
  void rename_proj_or_shape(std::istream & in, std::ostream & out, Projects & projs);
  void save(std::istream & in, std::ostream & out, Projects & projs);
  void save_as(std::istream & in, std::ostream & out, Projects & projs);
  void render(std::istream & in, std::ostream & out, Projects & projs);
  void merge(std::istream & in, std::ostream & out, Projects & projs);
  void save_all(std::ostream & out, Projects & projs);

  void create_rectangle(std::istream & in, std::ostream & out, Projects & projs);
  void create_concave(std::istream & in, std::ostream & out, Projects & projs);
  void create_complexquad(std::istream & in, std::ostream & out, Projects & projs);

  void delete_shape(std::istream & in, std::ostream & out, Projects & projs);
  void insert_shape(std::istream & in, std::ostream & out, Projects & projs);
  void swap_shapes(std::istream & in, std::ostream & out, Projects & projs);

  void sf(std::istream & in, std::ostream & out, Projects & projs);
  void sf_above(std::istream & in, std::ostream & out, Projects & projs);
  void sf_below(std::istream & in, std::ostream & out, Projects & projs);

  void move_abs(std::istream & in, std::ostream & out, Projects & projs);
  void move_rel(std::istream & in, std::ostream & out, Projects & projs);
  void move_all(std::istream & in, std::ostream & out, Projects & projs);
  void scale_shape(std::istream & in, std::ostream & out, Projects & projs);

  void print(std::ostream & out, Projects & projs);
  void print_shapes(std::istream & in, std::ostream & out, Projects & projs);
  void print_info_about_shape(std::istream & in, std::ostream & out, Projects & projs);

  void array_shapes(std::istream & in, std::ostream & out, Projects & projs);
  void reverse_project(std::istream & in, std::ostream & out, Projects & projs);
  void clear_project(std::istream & in, std::ostream & out, Projects & projs);

  void exit_program(std::istream & in, std::ostream & out, Projects & projs);
}

#endif
