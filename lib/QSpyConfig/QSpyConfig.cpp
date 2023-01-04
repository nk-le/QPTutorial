#include <qpcpp.hpp>
#include "QDefines.h"
#include <QSpyConfig.hpp>

using namespace QP;
using namespace QSPY_CONFIG;

#ifdef Q_SPY
namespace QP{

    void QP::QS::onReset(void) {

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
		//WRITE_RESTART(0x5FA0004);
		CPU_RESTART;

		// Note: _reboot_Teensyduino_() puts the teensy into programming mode, without re-starting the program.
		// _reboot_Teensyduino_();
	}

	// Set up buffers for QSPY, if it is enabled. Also opens QS_PORT port.
	bool QS::onStartup(void const* arg)
	{
		// Initialize buffers holding incoming and outgoing QSPY messages.
		DMAMEM static uint8_t qsTxBuf[4096]; // Create the buffer used for the tracing
		DMAMEM static uint8_t qsRxBuf[128];

		QP::QS::initBuf(qsTxBuf, sizeof(qsTxBuf));
		QP::QS::rxInitBuf(qsRxBuf, sizeof(qsRxBuf));


		// Open port.
		//if (!QS_PORT)	QS_PORT.begin(QS_BAUD);
		
		return true;
	}

	void QS::onCleanup(void) { ; }


	void QS::onFlush(void) {
		uint16_t len = 0xFFFFU; // big number to get as many bytes as available
		uint8_t const *buf = QS::getBlock(&len); // get continguous block of data
		while (buf != nullptr) { // data available?
			QS_PORT.write(buf, len); // might poll until all bytes fit
			len = 0xFFFFU; // big number to get as many bytes as available
			buf = QS::getBlock(&len); // try to get more data
		}
		QS_PORT.flush(); // wait for the transmission of outgoing data to complete
	}

	void __attribute__((weak)) QS::onCommand(unsigned char cmdId,
					unsigned long param1, unsigned long param2, unsigned long param3)
	{
	(void)param1;
	(void)param2;
	(void)param3;

	switch (cmdId) {
		case 0U: {
			break;
		}
		default:
			break;
		}
	}

	#ifndef Q_UTEST
    //............................................................................
    	void QP::QV::onIdle(void) { // called with interrupts DISABLED
		#ifdef NDEBUG
			// Put the CPU and peripherals to the low-power mode. You might
			// need to customize the clock management for your application,
			// see the datasheet for your particular MCU.
			QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
		#else
			QF_INT_ENABLE(); // simply re-enable interrupts


			// transmit QS outgoing data (QS-TX)
			uint16_t len = QS_PORT.availableForWrite();
			if (len > 0U) { // any space available in the output buffer?
				uint8_t const *buf = QS::getBlock(&len);
				if (buf) {
					QS_PORT.write(buf, len); // asynchronous and non-blocking
				}
			}

			// receive QS incoming data (QS-RX)
			len = QS_PORT.available();
			if (len > 0U) {
				do {
					QP::QS::rxPut(QS_PORT.read());
				} while (--len > 0U);
				QS::rxParse();
			}
		#endif
		}


    	// QUtest brings its own implementation of onGetTime (which has a special meaning)
		// Returns the current time of the micros() timer, which is used for QS timestamps.
		QSTimeCtr QS::onGetTime(void) { 
            return (unsigned long)micros();
        }
    #endif
}

#else 


#endif
