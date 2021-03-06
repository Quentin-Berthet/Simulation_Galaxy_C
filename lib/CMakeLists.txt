cmake_minimum_required(VERSION 3.14)
project(project C)

set(CMAKE_C_STANDARD 11)
include_directories("/usr/local/include")
set(CMAKE_INCLUDE_PATH /usr/local/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fsanitize=address -fsanitize=undefined")

add_executable(vector_tests vector_tests.c vector.c vector.h)
add_executable(box_tests box_tests.c box.c box.h vector.c vector.h)
add_executable(star_tests star_tests.c vector.c vector.h star.c star.h)
add_executable(node_tests node_tests.c node.c node.h box.c box.h star.c star.h vector.c vector.h)
add_executable(quad_tree_tests quad_tree_tests.c quad_tree.c quad_tree.h node.c node.h box.c box.h star.c star.h vector.c vector.h)
add_executable(galaxy_tests galaxy_tests.c galaxy.c galaxy.h box.c box.h star.c star.h vector.c vector.h)
add_executable(galaxy main.c gfx.c gfx.h quad_tree.c quad_tree.h node.c node.h galaxy.c galaxy.h box.c box.h star.c star.h vector.c vector.h)
target_link_libraries(galaxy m)
target_link_libraries(galaxy SDL2)
