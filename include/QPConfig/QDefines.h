#include <qpcpp.hpp>

enum Records {
        #ifdef Q_SPY
        LOG_VERBOSE =  QP::QSpyUserOffsets::QS_USER0,
        #else
        LOG_VERBOSE = 100,
        #endif
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        #ifdef Q_SPY
        LOG_TOPIC = QP::QSpyUserOffsets::QS_USER1,
        LOG_MISC_DATA_COLLECTION,
        #else
        LOG_TOPIC = 105
        #endif
};