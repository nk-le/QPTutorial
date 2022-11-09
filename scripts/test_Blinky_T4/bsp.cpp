//.$file${.::bsp.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: blinky_bsp-teeny4.qm
// File:  ${.::bsp.cpp}
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
//.$endhead${.::bsp.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include <qpcpp.hpp>   // QP-C++ framework
#include "blinky.hpp"  // Blinky application interface
#include "bsp.hpp"     // Board Support Package (BSP)
#include <Arduino.h>

#ifdef Q_SPY
    #include <qs.hpp>
#endif

using namespace QP;

//----------------------------------------------------------------------------
// QS facilities

// un-comment if QS instrumentation needed
//#define QS_ON

static QP::QSpyId const l_TIMER_ID = { 0U }; // QSpy source ID

//----------------------------------------------------------------------------
// BSP functions
enum {
   LED = QS_USER
};

//............................................................................
void BSP::init(void) {
    // initialize the hardware used in this sketch...
    // NOTE: interrupts are configured and started later in QF::onStartup()

#ifdef Q_SPY
    QS_INIT(nullptr);

    // output QS dictionaries...
    QS_OBJ_DICTIONARY(&l_TIMER_ID);
    QS_FUN_DICTIONARY(&ledOff);
    QS_FUN_DICTIONARY(&ledOn);   

    // setup the QS filters...
    QS_GLB_FILTER(QP::QS_SM_RECORDS); // state machine records
    QS_GLB_FILTER(QP::QS_AO_RECORDS); // active object records
    QS_GLB_FILTER(QP::QS_UA_RECORDS); // all user records
#endif

    QS_FUN_DICTIONARY(&QHsm::top);
    QS_USR_DICTIONARY(LED);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

}
//............................................................................
void BSP::ledOff(void) {
    digitalWrite(LED_BUILTIN, LOW);
    QS_BEGIN_ID(LED, AO_Blinky->m_prio)
       QS_U8(1, 0);
       digitalWrite()
    QS_END()
}
//............................................................................
void BSP::ledOn(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    QS_BEGIN_ID(LED, AO_Blinky->m_prio)
       QS_U8(1, 1);
    QS_END()
}

//----------------------------------------------------------------------------
// QF callbacks...
//
// NOTE: The usual source of system clock tick in ARM Cortex-M (SysTick timer)
// is aready used by the Arduino library. Therefore, this code uses a different
// hardware Timer1 of the Teensy 4 board for providing the system clock tick.
//
// NOTE: You can re-define the macros to use a different ATSAM timer/channel.
//
#include <TimerOne.h>  // Teensy Timer1 interface

#define TIMER1_CLCK_HZ  1000000
#define TIMER_HANDLER   T1_Handler

// interrupts.................................................................
void TIMER_HANDLER(void) {
    QF::TICK_X(0, &l_TIMER_ID); // process time events for tick rate 0
}
//............................................................................
void QF::onStartup(void) {
    // configure the timer-counter channel........
    Timer1.initialize(TIMER1_CLCK_HZ / BSP::TICKS_PER_SEC);
    Timer1.attachInterrupt(TIMER_HANDLER);
    // ...
}


#ifndef Q_UTEST

//............................................................................
void QV::onIdle(void) { // called with interrupts DISABLED
#ifdef NDEBUG
    // Put the CPU and peripherals to the low-power mode. You might
    // need to customize the clock management for your application,
    // see the datasheet for your particular MCU.
    QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
#else
    QF_INT_ENABLE(); // simply re-enable interrupts

#ifdef Q_SPY

    // transmit QS outgoing data (QS-TX)
    uint16_t len = Serial.availableForWrite();
    if (len > 0U) { // any space available in the output buffer?
        uint8_t const *buf = QS::getBlock(&len);
        if (buf) {
            Serial.write(buf, len); // asynchronous and non-blocking
        }
    }

    // receive QS incoming data (QS-RX)
    len = Serial.available();
    if (len > 0U) {
        do {
            QP::QS::rxPut(Serial.read());
        } while (--len > 0U);
        QS::rxParse();
    }

#endif // QS_ON

#endif
}


//............................................................................
extern "C" Q_NORETURN Q_onAssert(char const * const module, int location) {
    //
    // NOTE: add here your application-specific error handling
    //
    (void)module;
    (void)location;

    QF_INT_DISABLE(); // disable all interrupts
    BSP::ledOn();  // trun the LED on
    for (;;) { // freeze in an endless loop for now...
    }
}
#endif
