#pragma once

#include <Arduino.h>
#include <qpcpp.hpp>

#ifdef Q_UTEST
    #include <QSPort.hpp>

    using namespace QP;

    using QSPY_CONFIG::QS_PORT;

    //extern HardwareSerial& QS_PORT;


#else
    #error "Q_UTEST not defined but include QTestPort"

#endif
