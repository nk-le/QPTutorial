# Development Guid - Tutorial QP - PlatformIO

# A Minimal Start-Up

Project's structure
 * [Dependencies](./dir1)
    * [qp](./dir1/qp.ext)
 * [QPTutorial](./dir2)
    * [lib](./dir2/file21.ext)
    * [include](./dir2/file22.ext)
    * [src](./dir2/file23.ext)
    * [test](./dir2/file23.ext)
    
      platformio.ini
      
      [README](./README.md)
 
Note: 
  Environment
  
# Native arduino project
  
  

# Integrate QP into the project
Using QP requires
  - QP Source Code

  * - 2 ways to connect the QP Library:
    - 1. Clone the QP repo into the <lib/> of the project
    - 2. Clone the QP repo anywhere and configure the path in platformio.ini file of the project:
        E.g. lib_extra_dirs = ../Dependencies/ --> Recommend since there is a generate_src_filter script to declare the preprocessor
  
  ## QP Kernels
      * Users' definition of QP callback functions:
    - Kernel: QV, QK, QX
    - - HArdware/Project specific: declared in include/ Folder

  ## QSPY
    User should declare this function itself
  void QS::onCommand(unsigned char cmdId,
					unsigned long param1, unsigned long param2, unsigned long param3)



  ## QUTEST


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

  QRelay.py

  CustomTestRunner.py

  Integrating Qutest and Platformio

  Testing without platformio

# Notes on the platformio.ini file
  - Read chain+ --> Compiler find the neceesary source files

  - lib_deps = 
    include/

  Environment

  Testing QP Flags 

  Build and Source file

  Testing


# Warning
  WARNING: the .pio contains the included path --> new project might not be able to compile


TODO:
  Unstable testing. The test results are not 100% deterministic
  Connection of CustomTestRunner must be reworked --> Sometimes we observe that the com port of teensy does not reconnect
  COM port is still hard coded. Is there anyway to regconize the port automatically?
  Rework the flag adjustment of QP librariey dependencies
  Integrate automated document generation

Further improvement
  Remote cheking before merging --> setup CI pipeline
  Add unittest for the designed modules  

