# Development Guid - Tutorial QP - PlatformIO

# A Minimal Start-Up

Foldr Structure
** Project tree

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

# Integrate QP into the project
Using QP requires
  - QP Source Code

  * - 2 ways to connect the QP Library:
    - 1. Clone the QP repo into the <lib/> of the project
    - 2. Clone the QP repo anywhere and configure the path in platformio.ini file of the project:
        E.g. lib_extra_dirs = ../Dependencies/ --> Recommend since there is a generate_src_filter script to declare the preprocessor
  
  * Users' definition of QP callback functions:
  - Kernel: QV, QK, QX
    - - HArdware/Project specific: declared in include/ Folder

  - Q_SPY
  - Q_UTEST
    - - Sidenote:
  - check library.json in <qp> folder.
  - The extra script is used to generate the build flags and compiling options

  Link to book: file:///G:/Workspace/cuav/Documentation/PSiCC2.pdf

 1. Example: BlinkyAO
  To integrate QP, we need to declar the follwing callback functions of QP
    Q_onAssert()
    QP::QF::onStartup()
    QP::QV::onIdle()

# Integrate Q_SPY into the project

# Integrate Q_TEST into the project

# Unit Testing
  - The blinky object

# PlatformIO Linking explaination
  - Read chain+ --> Compiler find the neceesary source files

  - lib_deps = 
    include/


# Warning
  WARNING: the .pio contains the included path --> new project might not be able to compile

 