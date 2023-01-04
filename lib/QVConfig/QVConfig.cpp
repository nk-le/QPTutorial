#include <qpcpp.hpp>
#include "QDefines.h"
#include <QPConfig.hpp>
#include <QVConfig.hpp>

using namespace QP;

#include <string>
IntervalTimer _systick;


// systick handler............................................................
void sys_tick_handler() {
	QP::QF::TICK_X(0, (void*)0); // process time events for tick rate 0
}


namespace QP{
	void QF::onStartup(void) {

		uint8_t systickPriority = 128; // QF_AWARE_ISR_CMSIS_PRI
		constexpr auto _systickPeriod = 1.e6 / (double) (QPConfig::TICKS_PER_SECOND);
		//static constexpr char tr[] = "# SYSTICK RATE:" ;

		QS_BEGIN_ID(0, 0)
			QS_STR("\n##############################################\n");
			QS_STR("#             BOARD CONFIGURATION            #\n");
			QS_STR("##############################################\n");
			QS_STR("# TICKS_PER_SECOND:");
			QS_U16(6, QPConfig::TICKS_PER_SECOND);
			QS_STR("\n# CPU FREQUENCY:");
			QS_U32(9, F_CPU);
			QS_STR("\n Systick-Priority: ");
			QS_U8(3, systickPriority);
		QS_END();

		// Start systick generation with hardware timer and kernel-aware priority.
		_systick.priority(systickPriority);
		_systick.begin(sys_tick_handler, _systickPeriod);

		// enable cpu cycle counter
		//DEBUG_PORT.print(F("Enabling CPU cycle counter...\n"));
		ARM_DEMCR |= ARM_DEMCR_TRCENA;
		ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
	}
	//............................................................................


    #ifndef Q_UTEST
    //............................................................................
        #ifndef Q_SPY
            void QP::QV::onIdle(void) { // called with interrupts DISABLED
            #ifdef NDEBUG
                // Put the CPU and peripherals to the low-power mode. You might
                // need to customize the clock management for your application,
                // see the datasheet for your particular MCU.
                QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
            #else
                QF_INT_ENABLE(); // simply re-enable interrupts
            #endif
            }
        #endif

        // ............................................................................
        extern "C" Q_NORETURN Q_onAssert(char const * const module, int location) {
            //
            // NOTE: add here your application-specific error handling
            //
            (void)module;
            (void)location;

            QF_INT_DISABLE(); // disable all interrupts
            for (;;) { // freeze in an endless loop for now...
            }
        }
    #endif
}
