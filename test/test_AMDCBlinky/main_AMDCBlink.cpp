

// Adapting QP Examples to Teensy 4.1
#include <Arduino.h>
#include <ArduinoLog.h>
//#include <QPT4Config/BoardConfig.hpp>

#include "qpcpp.hpp"   // QP-C++ framework
#include <amdc_blinky.hpp>  // AMDC Blinky application interface
#include <bsp.hpp>     // Board Support Package (BSP)


using namespace QP;
extern QP::QActive * const AO_AMDCBlinky;

/// Maximal size message that QP Memory pool can handle
struct MediumType{
    uint8_t dummy[15];
};

//............................................................................
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    // Init port used for qspy
    Serial.begin(115200);
    // Initialize framework
	QP::QF::init();
	
	#ifdef Q_SPY
		QS_INIT(nullptr);
		// setup the QS filters...
		QS_GLB_FILTER(QP::QS_SM_RECORDS); // state machine records
		QS_GLB_FILTER(QP::QS_AO_RECORDS); // active object records
		QS_GLB_FILTER(QP::QS_UA_RECORDS); // all user records

		// Add HSM top state.
		QS_FUN_DICTIONARY(&QHsm::top);
	#endif

    

	// Initialize publish-subscribe system
	static QP::QSubscrList subscrSto[AMDC_MAX_SIG];
	QF::psInit(subscrSto, Q_DIM(subscrSto)); 

	// Initialize event pools... Attention: this has to be called AFTER QS_INIT
	DMAMEM static QF_MPOOL_EL(MediumType) smlPoolSto[100];
	QP::QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // expect pause
    QS_TEST_PAUSE();

    // Declare logging
    QS_GLB_FILTER(QS_ALL_RECORDS);
    QS_GLB_FILTER(-QS_QF_TICK);
    QS_GLB_FILTER(AMDCRecords::INFO);
    QS_GLB_FILTER(AMDCRecords::WARNING);
    QS_GLB_FILTER(AMDCRecords::TESTING);
    QS_GLB_FILTER(AMDCRecords::ERROR);

    // dictionaries...
    QS_USR_DICTIONARY(AMDCRecords::INFO);
    QS_USR_DICTIONARY(AMDCRecords::WARNING);
    QS_USR_DICTIONARY(AMDCRecords::TESTING);
    QS_USR_DICTIONARY(AMDCRecords::ERROR);


    // Test Printing these out
    QS_BEGIN_ID(AMDCRecords::INFO, AO_AMDCBlinky->getPrio())
        QS_STR("Hello World");
    QS_END();

    uint8_t id[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    QS_BEGIN_ID(AMDCRecords::WARNING, AO_AMDCBlinky->getPrio())
        QS_STR("Device ID: ");
        QS_MEM(id, sizeof(id));
    QS_END();

    QS_BEGIN_ID(AMDCRecords::TESTING, AO_AMDCBlinky->getPrio())
        QS_STR("QUTEST checking ...");
    QS_END();

    QS_BEGIN_ID(AMDCRecords::ERROR, AO_AMDCBlinky->getPrio())
        QS_STR("ERROR checking ...");
    QS_END();

    // pause execution of the test and wait for the test script to continue
    //QS_TEST_PAUSE();

    // start the active objects...
    static QEvt const *blinkyQSto[100]; // event queue storage for Blinky
    AO_AMDCBlinky->start(1U,
                     blinkyQSto, Q_DIM(blinkyQSto),
                     nullptr, 0U, (QEvt *)0);
    //...

    //QS_OBJ_DICTIONARY(AO_Blinky);    
    QS_FUN_DICTIONARY(&QHsm::top);
    QS_FUN_DICTIONARY(&BSP::ledOn);
    QS_FUN_DICTIONARY(&BSP::ledOff);
    
    QS_SIG_DICTIONARY(TIMEOUT_SIG, AO_AMDCBlinky);

    QS_TEST_PAUSE();
}

//............................................................................
void loop() {
    QF::run(); // run the QF/C++ framework
}

/// Overriding the declared function for our specific test fixture
#ifdef Q_UTEST
void QS::onTestSetup(void) {
   
}

void QS::onTestTeardown(void) {
    
}

#endif

//............................................................................
//! callback function to execute user commands
#ifdef Q_SPY
    void QS::onCommand(std::uint8_t cmdId,
            std::uint32_t param1, std::uint32_t param2, std::uint32_t param3)
    {
        (void)param1;
        (void)param2;
        (void)param3;
    }
#endif