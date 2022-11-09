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

// //----------------------------------------------------------------------------
// // QS callbacks...
#ifdef Q_SPY

#ifdef Q_UTEST
void QS::onTestLoop() {
    rxPriv_.inTestLoop = true;


    while (rxPriv_.inTestLoop) {

        // receive QS incoming data (QS-RX)
        size_t len = Serial.available();
        if (len > 0) {
            do {
                QP::QS::rxPut(Serial.read());
            } while (--len > 0U);
            QS::rxParse();
        }
        
        // Transmit outgoing data
        if (Serial.availableForWrite() > 0) { // is TXE empty?
            uint16_t fifo = Serial.availableForWrite(); // max bytes we can write
            uint8_t const * block;     // max bytes we can write non-blocking
            block = getBlock(&fifo);
            Serial.write(block, fifo); // put all bytes into tx buffer
            Serial.flush();
            }
        }

    onFlush();

    // set inTestLoop to true in case calls to QS_onTestLoop() nest,
    // which can happen through the calls to QS_TEST_WAIT().
    rxPriv_.inTestLoop = true;
}
#endif


//............................................................................
bool QP::QS::onStartup(void const * arg) {
    static uint8_t qsTxBuf[1024]; // buffer for QS transmit channel (QS-TX)
    static uint8_t qsRxBuf[128];  // buffer for QS receive channel (QS-RX)
    initBuf  (qsTxBuf, sizeof(qsTxBuf));
    rxInitBuf(qsRxBuf, sizeof(qsRxBuf));
    Serial.begin(115200); // run serial port at 115200 baud rate
    // Serial.print("QS buffer initialized\n");
    return true; // return success
}
//............................................................................
void QP::QS::onCommand(uint8_t cmdId, uint32_t param1,
                       uint32_t param2, uint32_t param3)
{
    Serial.printf("Command %d received.\n", cmdId);
}


//............................................................................
void QP::QS::onCleanup(void) {
}

#ifndef Q_UTEST
//............................................................................
QP::QSTimeCtr QP::QS::onGetTime(void) {
    return millis();
}
#endif
//............................................................................
void QP::QS::onFlush(void) {
#ifdef Q_SPY
    uint16_t len = 0xFFFFU; // big number to get as many bytes as available
    uint8_t const *buf = QS::getBlock(&len); // get continguous block of data
    while (buf != nullptr) { // data available?
        Serial.write(buf, len); // might poll until all bytes fit
        len = 0xFFFFU; // big number to get as many bytes as available
        buf = QS::getBlock(&len); // try to get more data
    }
    Serial.flush(); // wait for the transmission of outgoing data to complete
#endif // QS_ON
}
//............................................................................
void QP::QS::onReset(void) {

    //WRITE_RESTART(0x5FA0004);
    CPU_RESTART;

    // Note: _reboot_Teensyduino_() puts the teensy into programming mode, without re-starting the program.
    //_reboot_Teensyduino_();
}

#endif // QS_ON
