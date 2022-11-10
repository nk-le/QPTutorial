#include <Arduino.h>
#include <qpcpp.hpp>  // QP/C++ framework API
#include "sensor.h" // Sensor application
#include <bsp.hpp>

using namespace QP;

//. Check for the minimum required QP version
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.8.0 or higher required
#endif

//.${AOs::AO_Sensor} .........................................................
Sensor dutSensor;
//.$enddef${AOs::AO_Sensor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


Sensor::Sensor()
  : QActive(&initial),
    m_timeEvt(this, TIMEOUT_SIG, 0U),
    m_samplingTimerEvt(this, SAMPLING_SIG, 0U)
{}

//.${AOs::Sensor::SM} ........................................................
Q_STATE_DEF(Sensor, initial) {
    QS_OBJ_DICTIONARY(&dutSensor);
    QS_OBJ_DICTIONARY(&dutSensor.m_timeEvt);
    QS_OBJ_DICTIONARY(&dutSensor.m_samplingTimerEvt);

    // arm the private time event to expire in 1/2s
    // and periodically every 1/2 second
    QP::QTimeEvtCtr nTicks = QPConfig::TICKS_PER_SECOND / 2;
    m_timeEvt.armX(nTicks, nTicks);

    // arm the sampling time event to expire in 1/2s
    // and periodically every 1/10 second
    QP::QTimeEvtCtr nSamplingTicks = QPConfig::TICKS_PER_SECOND / 10;
    m_samplingTimerEvt.armX(nSamplingTicks, nSamplingTicks);
    (void)e; // unused parameter

    QS_FUN_DICTIONARY(&Sensor::off);
    QS_FUN_DICTIONARY(&Sensor::on);

    return tran(&off);
}
//.${AOs::Sensor::SM::off} ...................................................
Q_STATE_DEF(Sensor, off) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::Sensor::SM::off}
        case Q_ENTRY_SIG: {
            BSP::ledOff();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${AOs::Sensor::SM::off::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = tran(&on);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.${AOs::Sensor::SM::on} ....................................................
Q_STATE_DEF(Sensor, on) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::Sensor::SM::on}
        case Q_ENTRY_SIG: {
            BSP::ledOn();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${AOs::Sensor::SM::on::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = tran(&off);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.$enddef${AOs::Sensor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
