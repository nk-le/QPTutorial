#include "qpcpp.hpp" // for QUTEST
#include <Arduino.h>
#include <Defines.h>
#include <bsp.hpp>

#include <QDefines.h>

// QP Project Configuration
#include <QPConfig.hpp>
#include <QVConfig.hpp>
#include <QutestConfig.hpp>

namespace QSPY_CONFIG{
    Stream& QS_PORT = QSPY_PORT;
    int QS_BAUD = QSPY_BAUD;
};

using namespace QP;

Q_DEFINE_THIS_MODULE("test_uartSensor")
#include <sensor.h>
extern Sensor dutSensor;

//----------------------------------------------------------------------------
static uint8_t buffer[100];
static uint32_t myFun(void);

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

    // Declare dictionary
    DeclareQRecord();

    // global filter
    QS_GLB_FILTER(QP::QS_ALL_RECORDS); // enable all QS records

    // dictionaries...
    QS_OBJ_DICTIONARY(buffer);
    QS_FUN_DICTIONARY(&myFun);

    // Setup devices
    dutSensor.setup();
    dutSensor.testConnection();
    dutSensor.testParsing();
    QS_TEST_PAUSE();

    // Start AO;
    static QEvt const *sensorQSto[100]; // event queue storage for active object
    dutSensor.start(1U, sensorQSto, Q_DIM(sensorQSto), nullptr, 0U, (QEvt *)0);

}

void loop(){
    QF::run(); // run the tests
}

static uint32_t myFun(void) {
    QS_TEST_PROBE_DEF(&myFun)
    QS_TEST_PROBE(
        return qs_tp_;
    )
    return 0;
}
