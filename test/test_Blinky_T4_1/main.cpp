

//.$file${.::blinky_bsp-teensy4.ino} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: blinky_bsp-teeny4.qm
// File:  ${.::blinky_bsp-teensy4.ino}
//
// This code has been generated by QM 5.1.3 <www.state-machine.com/qm/>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//.$endhead${.::blinky_bsp-teensy4.ino} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include <Arduino.h>
#include "qpcpp.hpp"   // QP-C++ framework
#include "blinky.hpp"  // Blinky application interface
#include "bsp.hpp"     // Board Support Package (BSP)


using namespace QP;

//............................................................................
void setup() {

    // Wait for USB-port to reconnect.
    auto startTime = millis();
    auto lastState = LOW;
    auto nextState = LOW;

    // Start-up blink
    while (millis() - startTime < 3000) {
        delay(200);

        lastState == LOW ? nextState = HIGH : nextState = LOW;

        digitalWrite(LED_BUILTIN, nextState);
        lastState = nextState;
    }


    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; // storage for small pool
    static QEvt const *blinkyQSto[10]; // event queue storage for Blinky

    QF::init(); // initialize the framework
    BSP::init(); // initialize the BSP

    QS_GLB_FILTER(QS_ALL_RECORDS);

    // dictionaries...
    QS_SIG_DICTIONARY(TIMEOUT_SIG, nullptr);

    // QS_BEGIN_ID(QS_USER, 0)
    //     QS_STR("PAUSING TEST");
    // QS_END();

    // BSP::ledOff();

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

     // initialize event pools...
    QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start the active objects...
    AO_Blinky->start(1U,
                     blinkyQSto, Q_DIM(blinkyQSto),
                     nullptr, 0U, (QEvt *)0);
    //...

    //QS_OBJ_DICTIONARY(AO_Blinky);
    QS_FUN_DICTIONARY(&QHsm::top);
    QS_FUN_DICTIONARY(&BSP::ledOn);
    QS_FUN_DICTIONARY(&BSP::ledOff);
    QS_SIG_DICTIONARY(TIMEOUT_SIG, AO_Blinky);

    QF::run(); // run the QF/C++ framework
}

//............................................................................
void loop() {
    QF::run(); // run the QF/C++ framework
}


namespace QSPY_CONFIG{
    //HardwareSerial & getQSPYSerial();
    Stream& QS_PORT = Serial;
    int QS_BAUD = 115200;
};

#ifdef Q_UTEST
// //............................................................................
void QS::onTestSetup(void) {
   
}
//............................................................................
void QS::onTestTeardown(void) {
    
}

#endif

//............................................................................
//! callback function to execute user commands
void QS::onCommand(std::uint8_t cmdId,
         std::uint32_t param1, std::uint32_t param2, std::uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;
}

// #ifdef Q_UTEST
// //----------------------------------------------------------------------------
// void QS::onTestSetup(void) {
// }
// //............................................................................
// void QS::onTestTeardown(void) {
// }
// //............................................................................
// // callback function to "massage" the event, if necessary
// void QS::onTestEvt(QEvt *e) {
//     (void)e;
// #ifdef Q_HOST  // is this test compiled for a desktop Host computer?
// #else // this test is compiled for an embedded Target system
// #endif
// }
// //............................................................................
// // callback function to output the posted QP events (not used here)
// void QS::onTestPost(void const *sender, QActive *recipient,
//                     QEvt const *e, bool status)
// {
//     (void)sender;
//     (void)recipient;
//     (void)e;
//     (void)status;
// }


//#endif