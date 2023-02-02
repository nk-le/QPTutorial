#include <Arduino.h>
#include <ArduinoLog.h>
#include <MathCore/Types/BasicTypes.h>
#include <MISC_HW/LEDCtrl.hpp>

#include <qpcpp.hpp> // Import QP Framework
#include <bsp.hpp>

using namespace QP;

void setup(){
    blink(200,20);
    Serial.begin(115200);

    // Setup QP framework
    BSP::init();

    // Initialize the active objects
    // start the active objects...
    static QEvt const *blinkyQSto[100]; // event queue storage for Blinky
    AO_Blinky->start(1U, blinkyQSto, Q_DIM(blinkyQSto), nullptr, 0U, (QEvt *)0);
}

void loop(){
    QF::run();
}
