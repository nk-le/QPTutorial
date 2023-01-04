#include <Arduino.h>
#include <Defines.h>

#include <qpcpp.hpp>

#include <bsp.hpp>
#include <blinky.hpp>

// QP Project Configuration
#include <QPConfig.hpp>
#include <QVConfig.hpp>
#include <QSpyConfig.hpp>

using namespace QP;

Q_DEFINE_THIS_FILE

//............................................................................
namespace QSPY_CONFIG{
    Stream& QS_PORT = Serial;
    int QS_BAUD = 115200;
};

void setup() {
    Serial.begin(QSPY_CONFIG::QS_BAUD);

    // init framework
    QPConfig::init();
    BSP::init();

    // dictionaries...
    QS_SIG_DICTIONARY(TIMEOUT_SIG, nullptr);

    // initialize event pools...
    // start the active objects...
    static QEvt const *blinkyQSto[10]; // event queue storage for Blinky
    AO_Blinky->start(1U,
                     blinkyQSto, Q_DIM(blinkyQSto),
                     nullptr, 0U, (QEvt *)0);

}

void loop(){
    QF::run();
}

