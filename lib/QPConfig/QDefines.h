#pragma once
#include <qpcpp.hpp>


enum RecordID {
    #ifdef Q_SPY
    LOG_VERBOSE =  QP::QSpyUserOffsets::QS_USER,
    #else
    LOG_VERBOSE = 100,
    #endif
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    #ifdef Q_SPY
    LOG_TOPIC = QP::QSpyUserOffsets::QS_USER0,
    LOG_MISC_DATA_COLLECTION,
    #else
    LOG_TOPIC = 105,
    #endif
    LOG_MAX = 110
};


enum CommandID{
    FIXTURE_SETUP = RecordID::LOG_MAX,
    FIXTURE_TEARDOWN,
    COMMAND_A,
    COMMAND_B,
    COMMAND_X,
    COMMAND_Y,
    COMMAND_Z,
    MY_RECORD
};

void DeclareQRecord();
