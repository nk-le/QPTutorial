#include "qpcpp.hpp"  // QP/C++ framework API
#include "amdc_blinky.hpp" // AMDCBlinky application
#include "bsp.hpp"    // Board Support Package interface
#include <Arduino.h>

#include <qassert.h>
Q_DEFINE_THIS_FILE

using namespace QP;

// ask QM to declare the AMDCBlinky class ----------------------------------------
//.$declare${AOs::AMDCBlinky} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//.${AOs::AMDCBlinky} ............................................................
class AMDCBlinky : public QP::QActive {
private:
    QP::QTimeEvt m_timeEvt;

    void handleChangeFreqSig(const QEvt* e);

public:
    static AMDCBlinky inst;

public:
    AMDCBlinky();

protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(active);
        Q_STATE_DECL(off);
        Q_STATE_DECL(on);
};
//.$enddecl${AOs::AMDCBlinky} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// define the global opaque pointer to AMDCBlinky AO -----------------------------
//.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//. Check for the minimum required QP version
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.8.0 or higher required
#endif
//.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//.$define${AOs::AO_Blinky} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

// global "opaque" pointer to the AMDCBlinky AO
//.${AOs::AO_Blinky} .........................................................
QP::QActive * const AO_AMDCBlinky = &AMDCBlinky::inst;
//.$enddef${AOs::AO_Blinky} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// ask QM to define the AMDCBlinky class (including the state machine) -----------
//.$define${AOs::AMDCBlinky} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//.${AOs::AMDCBlinky} ............................................................
AMDCBlinky AMDCBlinky::inst;
//.${AOs::AMDCBlinky::AMDCBlinky} ....................................................
AMDCBlinky::AMDCBlinky()
  : QActive(&initial),
    m_timeEvt(this, TIMEOUT_SIG, 0U)
{}

void AMDCBlinky::handleChangeFreqSig(const QEvt* e){
    if(e->sig == ADJUST_FREQ_SIG){
        m_timeEvt.disarm();
        auto freq = Q_EVT_CAST(AdjustFreqLEDMessage)->freq;
        Q_ENSURE_ID(100, (freq > 0) && (freq < 20)); // frequency must be larger than 0
        QP::QTimeEvtCtr nTicks = BSP::TICKS_PER_SEC / freq; // Compute the new ticks
        m_timeEvt.armX(nTicks, nTicks);

        // Announce updating frequency
        QS_BEGIN_ID(AMDCRecords::TESTING, AMDCBlinky::inst.getPrio())
            QS_STR("LED f_new = ");
            QS_U16(0, freq);
        QS_END();
    }
}

//.${AOs::AMDCBlinky::SM} ........................................................
Q_STATE_DEF(AMDCBlinky, initial) {
    //.${AOs::AMDCBlinky::SM::initial}
    QS_OBJ_DICTIONARY(&AMDCBlinky::inst);
    QS_OBJ_DICTIONARY(&AMDCBlinky::inst.m_timeEvt);

    // arm the private time event to expire in 1/2s
    // and periodically every 1/2 second
    QP::QTimeEvtCtr nTicks = BSP::TICKS_PER_SEC / 2;
    m_timeEvt.armX(nTicks, nTicks);
    (void)e; // unused parameter

    QS_FUN_DICTIONARY(&AMDCBlinky::off);
    QS_FUN_DICTIONARY(&AMDCBlinky::on);
    QS_FUN_DICTIONARY(&AMDCBlinky::active);

    // declare the additional signals
    QS_SIG_DICTIONARY(ADJUST_FREQ_SIG, this);

    subscribe(ADJUST_FREQ_SIG);

    return tran(&off);
}

Q_STATE_DEF(AMDCBlinky, active) {
    switch (e->sig) {
        case Q_ENTRY_SIG:
            return Q_RET_HANDLED;
        case Q_EXIT_SIG:
            return Q_RET_HANDLED;
        case ADJUST_FREQ_SIG: {
            handleChangeFreqSig(e);
            return Q_RET_HANDLED;
        }
        default:
            return super(&top);
    };

}

//.${AOs::AMDCBlinky::SM::off} ...................................................
Q_STATE_DEF(AMDCBlinky, off) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::AMDCBlinky::SM::off}
        case Q_ENTRY_SIG: {
            BSP::ledOff();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${AOs::AMDCBlinky::SM::off::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = tran(&on);
            break;
        }
         case ADJUST_FREQ_SIG: {
            handleChangeFreqSig(e);
            return Q_RET_HANDLED;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.${AOs::AMDCBlinky::SM::on} ....................................................
Q_STATE_DEF(AMDCBlinky, on) {
    QP::QState status_;
    switch (e->sig) {
        //.${AOs::AMDCBlinky::SM::on}
        case Q_ENTRY_SIG: {
            BSP::ledOn();
            status_ = Q_RET_HANDLED;
            break;
        }
        //.${AOs::AMDCBlinky::SM::on::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = tran(&off);
            break;
        }
        case ADJUST_FREQ_SIG: {
            handleChangeFreqSig(e);
            return Q_RET_HANDLED;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//.$enddef${AOs::AMDCBlinky} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
