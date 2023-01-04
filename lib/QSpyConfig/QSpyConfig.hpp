#pragma once

#include <Arduino.h>
#include <qpcpp.hpp>

#include <HardwareSerial.h>

using namespace QP;

#ifdef Q_SPY
    namespace QSPY_CONFIG{
        extern Stream& QS_PORT;
        extern int QS_BAUD;
    };
#else 
    #error "Q_SPY not defined but include QSpyConfig.hpp"
#endif