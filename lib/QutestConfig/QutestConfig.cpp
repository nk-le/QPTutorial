#include <qpcpp.hpp>
#include <QDefines.h>
#include <QutestConfig.hpp>
#ifdef Q_UTEST
// #include <HardwareSerial.h>
// extern HardwareSerial& QS_PORT;

    #warning "Compiling Q_UTEST Port ..."
    #include <QutestConfig.hpp>
    #include <QSpyConfig.hpp>

    #define QP_IMPL        // this is QP implementation
    #include "qf_port.hpp" // QF port
    #include "qs_port.hpp" // QS port
    #include "qs_pkg.hpp"  // QS package-scope interface
    #include "qassert.h"   // QP embedded systems-friendly assertions
    #include <Arduino.h>

    using namespace QP;

    #define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
    #define CPU_RESTART_VAL 0x5FA0004
    #define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

    void QS::onTestLoop() {
        rxPriv_.inTestLoop = true;


        while (rxPriv_.inTestLoop) {

            // receive QS incoming data (QS-RX)
            size_t len = QS_PORT.available();
            if (len > 0) {
                do {
                    QP::QS::rxPut(QS_PORT.read());
                } while (--len > 0U);
                QS::rxParse();
            }
            
            // Transmit outgoing data
            if (QS_PORT.availableForWrite() > 0) { // is TXE empty?
                uint16_t fifo = QS_PORT.availableForWrite(); // max bytes we can write
                uint8_t const * block;     // max bytes we can write non-blocking
                block = getBlock(&fifo);
                QS_PORT.write(block, fifo); // put all bytes into tx buffer
                QS_PORT.flush();
                }
            }

        onFlush();

        // set inTestLoop to true in case calls to QS_onTestLoop() nest,
        // which can happen through the calls to QS_TEST_WAIT().
        rxPriv_.inTestLoop = true;
    }

    void __attribute__((weak))  QS::onTestSetup(void) {
    }
    
    //............................................................................
    void __attribute__((weak)) QS::onTestTeardown(void) {
    }
    //............................................................................

    //............................................................................
    // callback function to "massage" the event, if necessary
    void __attribute__((weak)) QS::onTestEvt(QEvt *e) {
    (void)e;
        #ifdef Q_HOST  // is this test compiled for a desktop Host computer?
        #else // this test is compiled for an embedded Target system
        #endif
    }
    //............................................................................
    // callback function to output the posted QP events (not used here)
    void __attribute__((weak)) QS::onTestPost(void const *sender, QActive *recipient,
            QEvt const *e, bool status)
    {
        (void)sender;
        (void)recipient;
        (void)e;
        (void)status;
    }

    //............................................................................
    void __attribute__((weak)) QP::QS::onCommand(uint8_t cmdId, uint32_t param1,
                        uint32_t param2, uint32_t param3)
    {

    }

#else

#endif