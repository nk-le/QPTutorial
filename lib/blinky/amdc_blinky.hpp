#pragma once
#include <qpcpp.hpp>
#include "blinky.hpp"

using namespace QP;

enum AMDCBlinkySignals {
    NONE_SIG = BlinkySignals::MAX_SIG,
    ADJUST_FREQ_SIG,
    AMDC_MAX_SIG               
};

struct AdjustFreqLEDMessage : QEvt {
    uint16_t freq = 0;
};

// global "opaque" pointer to the Blinky AO
extern QP::QActive * const AO_AMDCBlinky;