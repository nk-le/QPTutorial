#include <Arduino.h>
#include <Defines.h>

#include <qpcpp.hpp>
#include <QPConfig.hpp>

// QP Project Configuration
#include <bsp.hpp>

// QP Active Object under Test
#include <blinky.hpp>

using namespace QP;

Q_DEFINE_THIS_FILE

//............................................................................
void setup() {
    Serial.begin(0);
    QSPY_PORT.begin(QSPY_BAUD);

    // init framework
    QPConfig::init();
    BSP::init();

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

namespace QSPY_CONFIG{
    //HardwareSerial & getQSPYSerial();
    HardwareSerial& QS_PORT = QSPY_PORT;
    int QS_BAUD = QSPY_BAUD;
};