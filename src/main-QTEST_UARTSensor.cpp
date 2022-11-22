#include "qpcpp.hpp" // for QUTEST
#include <Arduino.h>
#include <Defines.h>
#include <QPConfig.hpp>
#include <bsp.hpp>

#include <QDefines.h>

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
// //............................................................................
void QS::onTestSetup(void) {
    QS_BEGIN_ID(CommandID::FIXTURE_SETUP, 0U)
    QS_END()
}
//............................................................................
void QS::onTestTeardown(void) {
    QS_BEGIN_ID(CommandID::FIXTURE_TEARDOWN, 0U)
    QS_END()
}

//............................................................................
//! callback function to execute user commands
void QS::onCommand(std::uint8_t cmdId,
         std::uint32_t param1, std::uint32_t param2, std::uint32_t param3)
{
    (void)param1;
    (void)param2;
    (void)param3;

    switch (cmdId) {
        case COMMAND_A: {
            Q_ASSERT_ID(100, param1 != 0U);
            QS_BEGIN_ID(COMMAND_A, 0U) // app-specific record
                QS_U32(0, param1);
            QS_END()
            break;
        }
        case COMMAND_B: {
            QS_BEGIN_ID(COMMAND_B, 0U) // app-specific record
                QS_U8(0, param1);
                QS_STR("BAR");
                QS_U16(0, param2);
                QS_STR("FOO");
                QS_U32(0, param3);
                QS_F64(param1, -6.02214076E23);
            QS_END()
            break;
        }
        case COMMAND_X: {
            uint32_t x = dutSensor.testProbe();
            QS_BEGIN_ID(COMMAND_X, 0U) // app-specific record
                QS_U32(0, x);
                // ...
            QS_END()
            break;
        }
        case COMMAND_Y: {
            QS_BEGIN_ID(COMMAND_Y, 0U) // application-specific record
                QS_FUN(&myFun);
                QS_MEM(buffer, param1);
                QS_STR((char const *)&buffer[33]);
            QS_END()
            break;
        }
        case COMMAND_Z: {
            float32_t f32 = (float32_t)((int32_t)param2/(float32_t)param3);
            float64_t f64 = -6.02214076E23;
            QS_BEGIN_ID(COMMAND_Z, 0U) /* app-specific record */
                QS_F32(param1, f32);
                QS_F64(param1, f64);
            QS_END()
            break;
        }
        default:
            break;
    }
}


//----------------------------------------------------------------------------
static uint32_t myFun(void) {
    QS_TEST_PROBE_DEF(&myFun)
    QS_TEST_PROBE(
        return qs_tp_;
    )
    return 0;
}

