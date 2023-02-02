

// Adapting QP Examples to Teensy 4.1
#include <Arduino.h>
#include <ArduinoLog.h>
//#include <QPT4Config/BoardConfig.hpp>

#include "qpcpp.hpp"   // QP-C++ framework
#include <blinky.hpp>  // Blinky application interface
#include <bsp.hpp>     // Board Support Package (BSP)

using namespace QP;


//............................................................................
void setup() {
    // Init port used for qspy
    Serial.begin(115200);

    // Wait for USB-port to reconnect.
    delay(500);
    BSP::init();

    // Declare logging
    QS_GLB_FILTER(QS_ALL_RECORDS);

    // dictionaries...
    QS_SIG_DICTIONARY(TIMEOUT_SIG, nullptr);

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

    // start the active objects...
    static QEvt const *blinkyQSto[100]; // event queue storage for Blinky
    AO_Blinky->start(1U,
                     blinkyQSto, Q_DIM(blinkyQSto),
                     nullptr, 0U, (QEvt *)0);
    //...

    //QS_OBJ_DICTIONARY(AO_Blinky);    
    QS_FUN_DICTIONARY(&QHsm::top);
    QS_FUN_DICTIONARY(&BSP::ledOn);
    QS_FUN_DICTIONARY(&BSP::ledOff);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, AO_Blinky);
}

//............................................................................
void loop() {
    QF::run(); // run the QF/C++ framework
}

/// Overriding the declared function for our specific test fixture
#ifdef Q_UTEST
void QS::onTestSetup(void) {
   
}

void QS::onTestTeardown(void) {
    
}

#endif

//............................................................................
//! callback function to execute user commands
#ifdef Q_SPY
    void QS::onCommand(std::uint8_t cmdId,
            std::uint32_t param1, std::uint32_t param2, std::uint32_t param3)
    {
        (void)param1;
        (void)param2;
        (void)param3;
    }
#endif