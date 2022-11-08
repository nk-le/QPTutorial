# Development Guid - Tutorial QP - PlatformIO

# A Minimal Start-Up

Foldr Structure
# Project tree

.
 * [tree-md](./tree-md)
 * [dir2](./dir2)
   * [file21.ext](./dir2/file21.ext)
   * [file22.ext](./dir2/file22.ext)
   * [file23.ext](./dir2/file23.ext)
 * [dir1](./dir1)
   * [file11.ext](./dir1/file11.ext)
   * [file12.ext](./dir1/file12.ext)
 * [file_in_root.ext](./file_in_root.ext)
 * [README.md](./README.md)
 * [dir3](./dir3)


- 2 ways to connect the QP Library:
    - 1. Clone the QP repo into the <lib/> of the project
    - 2. Clone the QP repo anywhere and configure the path in platformio.ini file of the project:
        E.g. lib_extra_dirs = ../Dependencies/
 