// #pragma once

// #include <qpcpp.hpp>
// #include <Arduino.h>

// namespace BoardConfig
// {

//     // Sys-tick rate used by QP as timer reference.
//     static constexpr auto TICKS_PER_SECOND = 50000U;

//     // The largest signal-id that QP expects.
//     static constexpr auto MAX_SIG = 600;

//     // Width of the QP message queue. No QP-message can be larger than this!
//     static constexpr auto MAX_MESSAGE_LEN = 512;

//     void blink(size_t periodDuration, size_t nPeriods);

//     void toggleLED();
//     size_t Get_IdleCounter();

//     void PrintAOQueues();
//     void PrintAOQueue(QP::QActive* AO);
//     void QSpyPrintAOInfo(QP::QActive* AO);
//     void QSpyPrintAOMinQueue(QP::QActive* AO);
    
//     // Initializes buffers and queues for the QP framework.
//     void init();

//     /// <summary>
//     /// Helper function to convert a duration in system ticks. Note: returns a minimum of 1 tick!
//     /// </summary>
//     /// <param name="durationInSeconds">The desired duration in seconds</param>
//     /// <returns>Number of ticks corresponding to the duration (or 1, if duration < systick duration)</returns>
//     constexpr QP::QTimeEvtCtr getTicksFromDuration(const double durationInSeconds)
//     {
//         auto ticksDouble = static_cast<double_t>(TICKS_PER_SECOND) * durationInSeconds;
//         auto nTicks = static_cast<QP::QTimeEvtCtr>(ceil(ticksDouble));
//         return QP::QTimeEvtCtr(max(nTicks, 1));
//     }



//     // Serial port used for sending debug-output.
//     // static constexpr usb_serial_class& DEBUG_PORT = Serial;
//     #define DEBUG_PORT Serial

//     // Dummy-baudrate specified when opening the debug serial port. Not used for USB-serial.
//     static constexpr auto DEBUG_BAUD = 115200U;

//     #ifdef Q_SPY
// 		// Use USB serial port also for QSPY
// 		// static constexpr usb_serial_class& QSPY_PORT = DEBUG_PORT;
// 		static constexpr HardwareSerial& QSPY_PORT = Serial3;
// 		static constexpr auto QSPY_BAUD = 1e6;
// 	#endif


//     /// <summary>
//     /// Dummy for a type with 512 byte for sizing the maximum message size.
//     /// </summary>
//     struct LargeType
//     {
//         uint8_t Dummy[512] = { 0U };
//     };
// }