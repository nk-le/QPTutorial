#pragma once 
#include "Arduino.h"
#include "HardwareSerial.h"
#include "qpcpp.hpp"

using namespace QP;

class SerialLink {
public:

    SerialLink();

    void begin(HardwareSerial* port, uint32_t baud);


private:


protected:
    HardwareSerial* port = nullptr;


};


class AO_SerialLink : public QP::QActive, public SerialLink{
private:
    QP::QTimeEvt m_sendingTimerEvt;
    QP::QTimeEvt m_offlineTimerEvt;

public:

public:
    AO_SerialLink();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(transmitting);
    Q_STATE_DECL(off);

};

extern AO_SerialLink dutSensor;
