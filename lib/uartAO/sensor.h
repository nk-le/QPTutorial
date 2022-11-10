#pragma once
#include <qpcpp.hpp>

using namespace QP;

enum BlinkySignals {
    DUMMY_SIG = QP::Q_USER_SIG,
    MAX_PUB_SIG,          // the last published signal

    TIMEOUT_SIG,
    SAMPLING_SIG,
    MAX_SIG               // the last signal
};


class Sensor : public QP::QActive {
private:
    QP::QTimeEvt m_timeEvt;
    QP::QTimeEvt m_samplingTimerEvt;

public:

public:
    Sensor();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(off);
    Q_STATE_DECL(on);
};

extern Sensor dutSensor;
