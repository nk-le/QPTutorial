#include <Arduino.h>
#include <qpcpp.hpp>  // QP/C++ framework API
#include "sensor.h" // Sensor application
#include <bsp.hpp>

#include <QDefines.h>

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
{
    
}


bool Sensor::setup(){
    QS_OBJ_DICTIONARY(&dutSensor);
    QS_OBJ_DICTIONARY(&dutSensor.m_timeEvt);
    QS_OBJ_DICTIONARY(&dutSensor.m_samplingTimerEvt);
    QS_FUN_DICTIONARY(&dutSensor.testProbe);

    QS_BEGIN_ID(RecordID::LOG_INFO, this->getPrio())
        QS_OBJ(this);
        QS_STR("Check Sensor ID");
        QS_STR("Firmware 0.1234");
    QS_END()
}

bool Sensor::testConnection(){
    delay(200); // You can use some delay here as well
    // For example: sending a sync request to SFU and expect to receive a confirmation!
    uint8_t rxBuf[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};

    QS_BEGIN_ID(RecordID::LOG_INFO, this->getPrio())
        QS_OBJ(this);
        QS_STR("Request Sample SFU");
        QS_MEM(rxBuf, sizeof(rxBuf));
    QS_END()
}


bool Sensor::testParsing(){
    delay(200);
    // For example: sending a sync request to SFU and expect to receive a confirmation!
    uint8_t rxBuf[12] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};

    QS_BEGIN_ID(RecordID::LOG_INFO, this->getPrio())
        QS_OBJ(this);
        QS_STR("Parse Bytes arr:");
        QS_MEM(rxBuf, sizeof(rxBuf));
        QS_STR("Result");
        QS_U64(0, 32003033); // Fake timestamp
        QS_U8(0, 122);
        QS_F64(0, 9.8299291);
        QS_F64(0, 4.2);
    QS_END()
}

bool Sensor::testProbe(){
    QS_TEST_PROBE_DEF(&Sensor::testProbe)
    QS_TEST_PROBE(        
        this->testConnection();
    )
}


bool Sensor::parse(uint8_t* rxBytes, uint8_t len){
    // Checksum

    // Casting

    // Return
    return true;
}

//.${AOs::Sensor::SM} ........................................................
Q_STATE_DEF(Sensor, initial) {
    

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
