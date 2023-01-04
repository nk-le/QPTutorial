#pragma once

#include <Arduino.h>
#include <qpcpp.hpp>

#ifdef Q_UTEST
    #include <QSpyConfig.hpp>
    using namespace QP;
    using QSPY_CONFIG::QS_PORT;
#else
    #error "Q_UTEST not defined but include QutestConfig.hpp"
#endif
