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
  PlatformIO Environment


# Integrate QP into the project
  Link to book: file:///G:/Workspace/cuav/Documentation/PSiCC2.pdf

  QP Website: ...

Using QP requires
  - QP Source Code

  - Installation of QTools

  - Add to Path

  * - 2 ways to connect the QP Library:
    - 1. Clone the QP repo into the <lib/> of the project
    - 2. Clone the QP repo anywhere and configure the path in platformio.ini file of the project:
        E.g. lib_extra_dirs = ../Dependencies/ --> Recommend since there is a generate_src_filter script to declare the preprocessor
  
  ## QP Kernels
      * Users' definition of QP callback functions:
    - Kernel: QV, QK, QX
    - - HArdware/Project specific: declared in include/ Folder

    QP::QF::TICK_X(0, (void*)0);

    void QF::onStartup(void);

    #ifndef Q_UTEST
    //........................................................
        #ifndef Q_SPY
            void QP::QV::onIdle(void) { // called with interrupts DISABLED
            #ifdef NDEBUG
                // Put the CPU and peripherals to the low-power mode. You might need to customize the clock management for your application, see the datasheet for your particular MCU.
                QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
            #else
                QF_INT_ENABLE(); // simply re-enable interrupts
            #endif
            }
        #endif

        // ............................................................................
        extern "C" Q_NORETURN Q_onAssert(char const * const module, int location) {
            //
            // NOTE: add here your application-specific error handling
            //
            (void)module;
            (void)location;

            QF_INT_DISABLE(); // disable all interrupts
            for (;;) { // freeze in an endless loop for now...
            }
        }
    #endif

  Examples: ...

  ## QSPY
    User should declare this function itself
    void __attribute__((weak)) QS::onCommand(unsigned char cmdId,
					unsigned long param1, unsigned long param2, unsigned long param3)

    bool QS::onStartup(void const* arg);

    void QS::onCleanup(void);

    void QS::onFlush(void);

    void __attribute__((weak)) QS::onCommand(unsigned char cmdId ,unsigned long param1, unsigned long param2, unsigned long param3)

    QSTimeCtr QS::onGetTime(void);

    #ifndef Q_UTEST
    void QP::QV::onIdle(void); // Redeclare of QVConfig to flush the qspy records
    #endif

  Examples: ...
    QSPY_BlinkyAO

  ## QUTEST

    void QS::onTestLoop();

    void __attribute__((weak))  QS::onTestSetup(void);
   
    void __attribute__((weak)) QS::onTestTeardown(void);

    void __attribute__((weak)) QS::onTestEvt(QEvt *e);

    void __attribute__((weak)) QS::onTestPost(void const *sender, QActive *recipient, QEvt const *e, bool status);

    void __attribute__((weak)) QP::QS::onCommand(uint8_t cmdId, uint32_t param1, uint32_t param2, uint32_t param3)

    Examples: ...
      QUTEST_SelfTest
      QUTEST_BlinkyAO
      QUTEST_UARTSensor

  ## Build Flags
    PlatformIO or CMake

    - Sidenote:
    - check library.json in <qp> folder.
  - The extra script is used to generate the build flags and compiling options






# Unit Testing
  - The blinky object

  ## Explaination of the testing procedure

  ## Testing ports 


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
  - [ ] Unstable testing. The test results are not 100% deterministic
  - [ ] Connection of CustomTestRunner must be reworked --> Sometimes we observe that the com port of teensy does not reconnect
  - [x] COM port is still hard coded. Is there anyway to regconize the port automatically?
  - [ ] Rework the flag adjustment of QP librariey dependencies
  - [ ] Integrate automated document generation
  - [ ] Check port clean up after testing/ reconnect port/ etc.

Further improvement
  - [ ] Remote cheking before merging --> setup CI pipeline
  - [ ] Add unittest for the designed modules  

