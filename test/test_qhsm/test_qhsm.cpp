//============================================================================
// Purpose: Fixture for QUTEST
// Last updated for version 6.9.1
// Last updated on  2020-09-21
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// Copyright (C) 2005-2020 Quantum Leaps. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses>.
//
// Contact information:
// <www.state-machine.com/licensing>
//============================================================================

#include <Arduino.h>
#include "qpcpp.hpp"
#include "qhsmtst.hpp"

#include <QPConfig.hpp>
#include <QutestConfig.hpp>

Q_DEFINE_THIS_FILE

using namespace QP;
using namespace QHSMTST;

enum {
    BSP_DISPLAY = QS_USER,
};

//----------------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);

    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; // small pool

    QF::init(); // initialize the framework and the underlying RT kernel

    // initialize the QS software tracing
    QS_INIT(nullptr);

    // initialize event pools...
    QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // dictionaries...
    QS_FUN_DICTIONARY(&QHsm::top);
    QS_OBJ_DICTIONARY(the_hsm);
    QS_USR_DICTIONARY(BSP_DISPLAY);

    QF::run();
}

void loop(){

}

//----------------------------------------------------------------------------

// void QS::onTestSetup(void) {
// }
// //............................................................................
// void QS::onTestTeardown(void) {
// }

//............................................................................
void QS::onCommand(uint8_t cmdId,
                   uint32_t param1, uint32_t param2, uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
       case 0U: {
           break;
       }
       default:
           break;
    }
}

//............................................................................
// callback function to "massage" the event, if necessary
// void QS::onTestEvt(QEvt *e) {
//     (void)e;
// #ifdef Q_HOST  // is this test compiled for a desktop Host computer?
// #else // this test is compiled for an embedded Target system
// #endif
// }
//............................................................................
// callback function to output the posted QP events (not used here)
// void QS::onTestPost(void const *sender, QActive *recipient,
//                     QEvt const *e, bool status)
// {
//     (void)sender;
//     (void)recipient;
//     (void)e;
//     (void)status;
// }

//----------------------------------------------------------------------------
namespace QHSMTST {

void BSP_display(char const *msg) {
    QS_BEGIN_ID(BSP_DISPLAY, 0U) // application-specific record
        QS_STR(msg);
    QS_END()
}
//............................................................................
void BSP_terminate(int16_t const result) {
    (void)result;
}

} // namespace QHSMTST


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