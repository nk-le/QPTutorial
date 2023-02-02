#include <Arduino.h>
#include <ArduinoLog.h>

#include <qpcpp.hpp> // Import QP Framework
#include <bsp.hpp> // Import QP Configuration

#include <MISC_HW/LEDCtrl.hpp>

#include<blinky.hpp>

using namespace QP;
int RecordMain = 105;

void setup(){
    blink(200,20);
    Serial.begin(115200);

    // Setup QP framework

    // BoardConfig::init();

    // Now we don't use the BoardConfig::init(); 
    // For other apps, you can define your own startup sequence like this ...
    BSP::init(); 
    
    // Initialuze the active objects
    delay(1000);

    QS_USR_DICTIONARY(RecordMain); // declare this so that the printed out logs have this name

    QS_STR("Current Counter: ");
    for(int i = 0; i < 50;i++){
        QS_BEGIN_ID(RecordMain, 0)
            QS_STR("Current Counter: ");
            QS_U8(0, i);
        QS_END();
        QS_FLUSH();
    }
    // Initialize the active objects
    QS_SIG_DICTIONARY(TIMEOUT_SIG, AO_Blinky);

    // start the active objects...
    static QEvt const *blinkyQSto[100]; // event queue storage for Blinky
    AO_Blinky->start(1U, blinkyQSto, Q_DIM(blinkyQSto), nullptr, 0U, (QEvt *)0);
}

void loop(){
    QF::run();
}

#ifdef Q_SPY
    // Suppose we don't want to use micros() for timestamp, but millis() instead.
    // We override the weak declaration in QSpyConfig by this way.
    QSTimeCtr QS::onGetTime(void) { 
        return (unsigned long)micros();
        //return (unsigned long)millis();
    }

    void QS::onCommand(unsigned char cmdId,
                    unsigned long param1, unsigned long param2, unsigned long param3)
    {
        (void)param1;
        (void)param2;
        (void)param3;
        // QS_BEGIN_ID(RecordMain, 0)
        //     QS_STR("Received Command: ");
        //     QS_U8(0, cmdId);
        // QS_END();


    switch (cmdId) {
        case 0U: {
            break;
        }
        case 1U:{
            break;
        }
        case 3U:{
            break;
        }
        default:
            break;
        }
    }
#endif