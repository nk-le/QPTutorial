// #include "BoardConfig.h"
// #include <IntervalTimer.h>
// #include <string>

// IntervalTimer _systick;

// using namespace QP;
// using namespace BoardConfig;

// #define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
// #define CPU_RESTART_VAL 0x5FA0004
// #define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);


// // systick handler............................................................
// void sys_tick_handler() {
// 	QP::QF::TICK_X(0, (void*)0); // process time events for tick rate 0
// }

// // Counter that is incremented each time QP::QV::onIdle is called.
// static volatile size_t idleCounter = 0;

// namespace QP
// {

// 	#ifdef Q_SPY
// 		// Use USB serial port also for QSPY
// 		static constexpr usb_serial_class& QSPY_PORT = DEBUG_PORT;
// 		static constexpr auto QSPY_BAUD = 6e6;
// 	#endif
		
// 	void QF::onStartup(void) {

// 		uint8_t systickPriority = 128; // QF_AWARE_ISR_CMSIS_PRI
// 		constexpr auto _systickPeriod = 1.e6 / (double) (BoardConfig::TICKS_PER_SECOND);
// 		//static constexpr char tr[] = "# SYSTICK RATE:" ;

// 		QS_BEGIN_ID(QS_USER, 0)
// 			QS_STR("\n##############################################\n");
// 			QS_STR("#             BOARD CONFIGURATION            #\n");
// 			QS_STR("##############################################\n");
// 			QS_STR("# TICKS_PER_SECOND:");
// 			QS_U16(6, BoardConfig::TICKS_PER_SECOND);
// 			QS_STR("\n# CPU FREQUENCY:");
// 			QS_U32(9, F_CPU);
// 			QS_STR("\n Systick-Priority: ");
// 			QS_U8(3, systickPriority);
// 		QS_END();

// 		// Start systick generation with hardware timer and kernel-aware priority.
// 		_systick.priority(systickPriority);
// 		_systick.begin(sys_tick_handler, _systickPeriod);

// 		// enable cpu cycle counter
// 		//DEBUG_PORT.print(F("Enabling CPU cycle counter...\n"));
// 		ARM_DEMCR |= ARM_DEMCR_TRCENA;
// 		ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
// 	}
// 	//............................................................................

// #ifdef Q_UTEST

// void QS::onTestLoop() {
//     rxPriv_.inTestLoop = true;


//     while (rxPriv_.inTestLoop) {

//         // receive QS incoming data (QS-RX)
//         size_t len = Serial.available();
//         if (len > 0) {
//             do {
//                 QP::QS::rxPut(Serial.read());
//             } while (--len > 0U);
//             QS::rxParse();
//         }
        
//         // Transmit outgoing data
//         if (Serial.availableForWrite() > 0) { // is TXE empty?
//             uint16_t fifo = Serial.availableForWrite(); // max bytes we can write
//             uint8_t const * block;     // max bytes we can write non-blocking
//             block = getBlock(&fifo);
//             Serial.write(block, fifo); // put all bytes into tx buffer
//             Serial.flush();
//             }
//         }

//     onFlush();

//     // set inTestLoop to true in case calls to QS_onTestLoop() nest,
//     // which can happen through the calls to QS_TEST_WAIT().
//     rxPriv_.inTestLoop = true;
// }


// #endif


// // Define QSPY-callpacks if QSPY is available.
// #ifdef Q_SPY

// void QP::QS::onReset(void) {

//         //WRITE_RESTART(0x5FA0004);
//         CPU_RESTART;

//         // Note: _reboot_Teensyduino_() puts the teensy into programming mode, without re-starting the program.
//         //_reboot_Teensyduino_();
// }

// #ifndef Q_UTEST // QUtest brings its own implementation of onGetTime (which has a special meaning)
//         // Returns the current time of the micros() timer, which is used for QS timestamps.
//         QSTimeCtr QS::onGetTime(void) { return (unsigned long)micros();}
// #endif

// // Set up buffers for QSPY, if it is enabled. Also opens Serial port.
// bool QS::onStartup(void const* arg)
// {
//         // Initialize buffers holding incoming and outgoing QSPY messages.
//         DMAMEM static uint8_t qsTxBuf[4096]; // Create the buffer used for the tracing
//         DMAMEM static uint8_t qsRxBuf[128];

//         QP::QS::initBuf(qsTxBuf, sizeof(qsTxBuf));
//         QP::QS::rxInitBuf(qsRxBuf, sizeof(qsRxBuf));


//         // Open port.
//         if (!QSPY_PORT)	QSPY_PORT.begin(QSPY_BAUD);
//         return true;
// }

// void QS::onCleanup(void) { ; }


// void QS::onFlush(void) {
//         uint16_t len = 0xFFFFU; // big number to get as many bytes as available
//         uint8_t const *buf = QS::getBlock(&len); // get continguous block of data
//         while (buf != nullptr) { // data available?
//                 QSPY_PORT.write(buf, len); // might poll until all bytes fit
//                 len = 0xFFFFU; // big number to get as many bytes as available
//                 buf = QS::getBlock(&len); // try to get more data
//         }
//         QSPY_PORT.flush(); // wait for the transmission of outgoing data to complete
// }
// #endif
// }



// size_t BoardConfig::Get_IdleCounter()
// {
// 	return idleCounter;
// }

// void BoardConfig::toggleLED()
// {
// 	static volatile bool _isOn;
// 	_isOn = !_isOn;

// 	if (_isOn)
// 	{
// 		digitalWrite(LED_BUILTIN, HIGH);
// 	}
// 	else
// 	{
// 		digitalWrite(LED_BUILTIN, LOW);
// 	}
// }

// // Blinks the internal LED for the specified number of times with the specified period duration (specified in [ms]).
// // Attention: blocking.
// void BoardConfig::blink(size_t periodDuration, size_t nPeriods)
// {
// 	pinMode(LED_BUILTIN, OUTPUT);
// 	digitalWrite(LED_BUILTIN, LOW);

// 	for (size_t i = 0; i < nPeriods; i++)
// 	{
// 		delay(periodDuration);
// 		toggleLED();
// 	}
// }

// /// <summary>
// /// For debugging purposes, print the all-time minimum space in the event queues of all registered AOs.
// /// </summary>
// void BoardConfig::PrintAOQueues()
// {

// 	DEBUG_PORT.print(F("Enumerating Active Objects...\n"));

// 	// Get a list of all AOs...
// 	auto AOs = QP::QF::active_;

// 	DEBUG_PORT.print("PRIORITY\tADDRESS\t\tSPACE_LEFT\n");

// 	// Iterate over all entries in the list, where the index equals the priority of the AO.
// 	for (size_t i = 0; i < QF_MAX_ACTIVE+1; i++)
// 	{
// 		if (! (AOs[i] == nullptr))
// 		{
// 			auto test = AOs[i];
// 			DEBUG_PORT.printf("%d,\t\t%d, \t\t%d\n", i, (intptr_t)test, QP::QF::getQueueMin(i));
// 		}
// 	}
// }

// /// <summary>
// /// Retrieves all events from the AO and prints their signal. Warning: this removes the events!
// /// </summary>
// void BoardConfig::PrintAOQueue(QP::QActive* AO)
// {

// 	auto iSignal = 0;

// 	DEBUG_PORT.printf("SIGNALS IN QUEUE OF AO WITH PRIORITY %d\n", AO->getPrio());
// 	while (! (AO->m_eQueue.isEmpty()))
// 	{
// 		auto evt = AO->get_();
// 		DEBUG_PORT.printf("%d: Signal=%d.\n", iSignal++, evt->sig);
// 	}
// }

// #ifdef Q_SPY
// void BoardConfig::QSpyPrintAOMinQueue(QP::QActive* AO){
// 	auto prio = AO->getPrio();
// 	QS_BEGIN_ID(Records::LOG_ERROR, 0)
// 		QS_STR("Min Queue Prio: ");
// 		QS_U8(0, prio);
// 		QS_U16(0, QP::QF::getQueueMin(prio));
// 	QS_END();
// 	QS::onFlush();
// }

// void BoardConfig::QSpyPrintAOInfo(QP::QActive* AO){
// 	auto prio = AO->getPrio();
// 	// Min Queue
// 	QS_BEGIN_ID(Records::LOG_ERROR, 0)
// 		QS_STR("INFO AO WITH PRIO: ");
// 		QS_U8(0, prio);
// 	QS_END();
// 	QS::onFlush();

// 	// Min Queue
// 	QSpyPrintAOMinQueue(AO);

// 	// Signal in Queue
// 	auto iSignal = 0;
// 	QS_BEGIN_ID(Records::LOG_ERROR, 0)
// 		QS_STR("Signals in queue:");
// 		QS_U8(0, AO->getPrio());
// 	QS_END();
// 	QS::onFlush();
// 	while (! (AO->m_eQueue.isEmpty())){
// 		iSignal++;
// 		auto evt = AO->get_();
// 		QS_BEGIN_ID(Records::LOG_ERROR, 0)
// 			QS_U16(0, iSignal);
// 			QS_STR("SIGNALS: ");
// 			QS_U16(0, evt->sig);
// 		QS_END();
// 		QS::onFlush();
// 	}
// }
// #endif 

// #ifndef Q_UTEST
// extern "C" Q_NORETURN Q_onAssert(char const* const module, int location) {
// 	//
// 	// NOTE: add here your application-specific error handling
// 	//

// 	//QF_INT_DISABLE(); // disable all interrupts

// 	_systick.end(); // disable the QP tick to print the actual signals saved in the buffer of AO
// 	auto AOs = QP::QF::active_;

//         #ifdef Q_SPY
//                 QS_BEGIN_ID(Records::LOG_ERROR, 0)
//                         QS_STR("QP ASSERTION in Module: ");
//                         QS_STR(module);
//                         QS_STR("Line: ");
//                         QS_I16(4, location);
//                 QS_END();
//                 QS::onFlush();

	
// 	for (size_t i = 0; i < QF_MAX_ACTIVE + 1; i++)
// 	{
// 		if (!(AOs[i] == nullptr)){
// 			BoardConfig::QSpyPrintAOInfo(AOs[i]);
// 		}
// 		QS::onFlush();
// 	}
//         #else

// 	for (size_t i = 1; i < QF_MAX_ACTIVE + 1; i++)
// 	{
// 		if (!(AOs[i] == nullptr))
// 		{
// 			//if (QP::QF::getQueueMin(i) == 0)
// 			//{
// 			BoardConfig::PrintAOQueue(AOs[i]);
// 			//}
// 		}
// 	}


// 	DEBUG_PORT.printf("QP ASSERTION in %s at line %d\n", module, location);
// 	DEBUG_PORT.printf("Min pool size: %d\n", QP::QF::getPoolMin(1));
// 	DEBUG_PORT.println("Minimum queue sizes of active objects: ");

// 	BoardConfig::PrintAOQueues();

// 	for (size_t i = 1; i < QF_MAX_ACTIVE + 1; i++)
// 	{
// 		if (!(AOs[i] == nullptr))
// 		{
// 			//if (QP::QF::getQueueMin(i) == 0)
// 			//{
// 			BoardConfig::PrintAOQueue(AOs[i]);
// 			//}
// 		}
// 	}

// 	DEBUG_PORT.flush();

//         #endif

// 	for (; ;)
// 	{
// 		//digitalWrite(LED_BUILTIN, LOW);
// 	}


// }

// #endif


// void BoardConfig::init() {

// 	// Initialize framework
// 	QP::QF::init();

// 	#ifdef Q_SPY
// 		QS_INIT(nullptr);
// 		// setup the QS filters...
// 		QS_GLB_FILTER(QP::QS_SM_RECORDS); // state machine records
// 		QS_GLB_FILTER(QP::QS_AO_RECORDS); // active object records
// 		QS_GLB_FILTER(QP::QS_UA_RECORDS); // all user records

// 		// Add HSM top state.
// 		QS_FUN_DICTIONARY(&QP::QHsm::top);
// 	#endif

	
// 	// // init publish-subscribe system
// 	static QP::QSubscrList subscrSto[MAX_SIG];
// 	QF::psInit(subscrSto, Q_DIM(subscrSto));

// 	// // initialize event pools... Attention: for some reason this has to be called AFTER QS_INIT
// 	DMAMEM static QF_MPOOL_EL(BoardConfig::LargeType) smlPoolSto[200];
// 	QP::QF::poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

// }