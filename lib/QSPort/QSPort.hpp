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

#endif