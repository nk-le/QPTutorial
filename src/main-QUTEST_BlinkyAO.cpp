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

    DMAMEM static uint8_t txBuf[4096]; // Create the buffer used for the tracing
	DMAMEM static uint8_t rxBuf[1024];
    QSPY_PORT.addMemoryForWrite(txBuf, sizeof(txBuf));
    QSPY_PORT.addMemoryForRead(rxBuf, sizeof(rxBuf));
    QSPY_PORT.begin(QSPY_BAUD);

    // init framework
    QPConfig::init();
    BSP::init();

    // dictionaries...
    QS_SIG_DICTIONARY(TIMEOUT_SIG, nullptr);

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

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
    Stream& QS_PORT = QSPY_PORT;
    int QS_BAUD = QSPY_BAUD;
};

#ifdef Q_UTEST
// //............................................................................
void QS::onTestSetup(void) {
   
}
//............................................................................
void QS::onTestTeardown(void) {
    
}

#endif

//............................................................................
//! callback function to execute user commands
void QS::onCommand(std::uint8_t cmdId,
         std::uint32_t param1, std::uint32_t param2, std::uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;
}