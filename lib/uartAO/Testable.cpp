// #include "Testable.hpp"

// enum TestSignals {
//     DUMMY_SIG = QP::Q_USER_SIG,

//     TEST_SIG,
//     MAX_SIG               // the last signal
// };

// // Plain C++ object
// Testable::Testable(){};

// void Testable::testStartup(){
//     QS_BEGIN_ID(0, getAOprio())
//         QS_STR("Setup...");
//         QS_U8(0, 123);
//         QS_STR("Version");
//     QS_END();
// }

// void Testable::testRun(){
//     QS_BEGIN_ID(0, getAOprio())
//         QS_STR("Testing");
//         QS_U8(0, 123);
//         QS_STR("Version");
//     QS_END();
// }

// void Testable::testClose(){
//     QS_BEGIN_ID(0, getAOprio())
//         QS_STR("End");
//     QS_END();
// }

// uint32_t Testable::testProbe()
// {
//     QS_TEST_PROBE_DEF(&testProbe)
//     QS_TEST_PROBE(
//         return qs_tp_;
//     )
// }