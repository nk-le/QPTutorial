//***************************************************************************
// Product: BSP for "Blinky" example
// Last updated for version 6.3.1
// Last updated on  2018-05-21
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) Quantum Leaps, LLC. All rights reserved.
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
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#ifndef BSP_HPP
#define BSP_HPP
#include <QPT4Config/BoardConfig.hpp>
#include <assert.h>

// a very simple Board Support Package (BSP) -----========--------------------
class BSP {
public:
    enum { TICKS_PER_SEC = QP_RATE} ;
    static void init(void);
    static void ledOff(void);
    static void ledOn(void);
};

// active object(s) used in this application -------------------------------
extern QP::QActive * const AO_Blinky; // opaque pointer to the Blinky AO

enum AMDCRecords {
    #ifdef Q_SPY
        INFO = QP::QSpyUserOffsets::QS_USER0,
    #else 
        INFO = 100,
    #endif
    WARNING,
    TESTING,
    ERROR,
    LOG_MAX = 124
};
#endif // BSP_HPP
