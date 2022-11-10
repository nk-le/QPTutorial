#include "txLink.hpp"
#include <bsp.hpp>

enum BlinkySignals {
    DUMMY_SIG = QP::Q_USER_SIG,

    MAX_PUB_SIG,          // the last published signal

    TRANSMIT_SIG,
    OFFLINE_SIG,
    MAX_SIG               // the last signal
};

// Plain C++ object
SerialLink::SerialLink(){};

void SerialLink::begin(HardwareSerial* ser, uint32_t baud){
    // Initalize serial port
    static uint8_t txMem[100];
    ser->addMemoryForWrite(txMem, sizeof(txMem));
    ser->begin(baud);

    // Print out sth as checking Serial was setup already
    // ...
}


// Wrapper - QP Active Object
AO_SerialLink dutLink;

AO_SerialLink::AO_SerialLink() : QActive(&initial),
    m_sendingTimerEvt(this, TRANSMIT_SIG, 0U), m_offlineTimerEvt(this, OFFLINE_SIG, 0U)
{}

Q_STATE_DEF(AO_SerialLink, initial) {
    (void)e; // unused parameter

    QS_OBJ_DICTIONARY(&dutLink);
    QS_OBJ_DICTIONARY(&dutLink.m_sendingTimerEvt);
    QS_OBJ_DICTIONARY(&dutLink.m_offlineTimerEvt);

    // Sending data over UART periodically
    QP::QTimeEvtCtr nTicks = QPConfig::TICKS_PER_SECOND / 2;
    m_sendingTimerEvt.armX(nTicks, nTicks);

    // offline after 10 seconds
    QP::QTimeEvtCtr nSamplingTicks = 10 * QPConfig::TICKS_PER_SECOND;
    m_offlineTimerEvt.armX(nSamplingTicks, nSamplingTicks);

    QS_FUN_DICTIONARY(&AO_SerialLink::off);
    QS_FUN_DICTIONARY(&AO_SerialLink::transmitting);

    return tran(&off);
}
//.${AOs::Sensor::SM::off} ...................................................
Q_STATE_DEF(AO_SerialLink, off) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::Sensor::SM::off}
        case Q_ENTRY_SIG: {
            status_ = Q_RET_HANDLED;
            break;
        }
        case TRANSMIT_SIG:
        {
            // Send sth here
            return Q_RET_HANDLED;
        }
        
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}


Q_STATE_DEF(AO_SerialLink, transmitting) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::Sensor::SM::off}
        case Q_ENTRY_SIG: {
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${AOs::Sensor::SM::off::TIMEOUT}
        case OFFLINE_SIG: {
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